import ROOT
import uuid
ROOT.gROOT.SetStyle('ATLAS')

_months = [
    'Jan.',
    'Feb.',
    'Mar.',
    'Apr.',
    'May',
    'Jun.',
    'Jul.',
    'Aug.',
    'Sep.',
    'Oct.',
    'Nov.',
    'Dec.',
]
def max_hist(h1, h2):
    return max(h1.GetBinContent(h1.GetMaximumBin()), h2.GetBinContent(h2.GetMaximumBin()))

def min_hist(h1, h2):
    return max(h1.GetBinContent(h1.GetMinimumBin()), h2.GetBinContent(h2.GetMinimumBin()))

def get_hist(name, n_days, color=1, linestyle=1, initial_date = 'Jan. 22'):
    h = ROOT.TH1F(name, name, n_days, 0, n_days - 1)
    h.SetLineColor(color)
    h.SetLineStyle(linestyle)
    h.SetMarkerColor(color)
    h.SetMarkerSize(0)
    h.SetLineWidth(4)
    h.GetYaxis().SetTitle('Cases')
    h.GetXaxis().SetTitle('Number of Days Since {}'.format(initial_date))
    return h

def chi2_test(hnew, href, do_plot=False):
    content_1 = [hnew.GetBinContent(i + 1) for i in xrange(hnew.GetNbinsX())]
    content_2 = [href.GetBinContent(i + 1) for i in xrange(href.GetNbinsX())]
    zero_suppressed_1 = []
    zero_suppressed_2 = []
    for c_1, c_2 in zip(content_1, content_2):
        if c_1 == 0 or c_2 == 0:
            continue
        if c_2 < 10:
            continue
        zero_suppressed_1 += [c_1]
        zero_suppressed_2 += [c_2]
    h_zero_suppressed_1 = ROOT.TH1F(uuid.uuid4().hex, '', len(zero_suppressed_1), 0, len(zero_suppressed_1) - 1)
    h_zero_suppressed_2 = ROOT.TH1F(uuid.uuid4().hex, '', len(zero_suppressed_2), 0, len(zero_suppressed_2) - 1)

    for i_c, (c_1, c_2) in enumerate(zip(zero_suppressed_1, zero_suppressed_2)):
        h_zero_suppressed_1.SetBinContent(i_c + 1, c_1)
        h_zero_suppressed_2.SetBinContent(i_c + 1, c_2)

    _res = h_zero_suppressed_1.Chi2Test(h_zero_suppressed_2, 'UUCHI2/NDF')
    _scaling = h_zero_suppressed_2.Integral() / h_zero_suppressed_1.Integral()
    if do_plot:
        c = ROOT.TCanvas()
        #     c.SetLogy()
        #     h_zero_suppressed_1.Scale(1. / h_zero_suppressed_1.Integral())
        #     h_zero_suppressed_2.Scale(1. / h_zero_suppressed_2.Integral())
        #     h_zero_suppressed_1.GetYaxis().SetRangeUser(
        #         min_hist(h_zero_suppressed_1, h_zero_suppressed_2) - 10, 
        #         max_hist(h_zero_suppressed_1, h_zero_suppressed_2) + 10)
        h_zero_suppressed_1.Draw('HIST')
        h_zero_suppressed_2.SetLineColor(2)
        h_zero_suppressed_2.Draw('sameHIST')
        c.SaveAs('{}_zerosuppressed.pdf'.format(h1.GetName()))

    return _res, h_zero_suppressed_2.Integral() / h_zero_suppressed_1.Integral()

#-----------------------
if __name__ == '__main__':
    _ref_country = 'Italy'
    _new_country = 'France'
    _x_min_for_plotting = 30
    _N_OFFSET = 20


    rfile = ROOT.TFile('data_covid19.root')
    
    tree = rfile.Get('tree')
    
    h_ref = get_hist(_ref_country, tree.GetEntries(), ROOT.kRed)
    h_new = get_hist(_new_country, tree.GetEntries(), ROOT.kBlue)
    
    h_new_offsets = []
    for i in xrange(_N_OFFSET):
        h = get_hist('{}_minus_{}'.format(_new_country, i), tree.GetEntries(), ROOT.kBlue)
        h_new_offsets.append(h)

    # this is the main event loop
    for i_evt, event in enumerate(tree):
        print 'Read ', event.date

        _cases_ref = 0
        _cases_new = 0
        for _country, _province, _cases in zip(event.Country, event.Province, event.Cases):
            if _country == _new_country: 
                _cases_new += _cases

            if _country == _ref_country:
                _cases_ref += _cases

        h_ref.SetBinContent(i_evt + 1, _cases_ref)
        h_new.SetBinContent(i_evt + 1, _cases_new)
        for offset in xrange(_N_OFFSET):
            if i_evt - offset >= 0:
                h_new_offsets[offset].SetBinContent(i_evt - offset + 1, _cases_new)


    _chi2_hist = ROOT.TGraph(_N_OFFSET)
    _chi2_hist.GetXaxis().SetTitle('Offset')
    _chi2_hist.GetYaxis().SetTitle('#chi^{2} / NDF (Arbitrary Unit)')
    _chi2_hist.SetMarkerStyle(ROOT.kDot)
    _chi2_hist.SetMarkerSize(1)
    _chi2_hist.SetLineWidth(3)
    offset_min = -1
    _chi2_min = 1e9
    _scales = []
    for offset in xrange(_N_OFFSET):
        _chi2, _scale = chi2_test(h_new_offsets[offset], h_ref)
        _scales.append(_scale)
        _chi2_hist.SetPoint(offset + 1, offset, _chi2)
        if _chi2 < _chi2_min:
            _chi2_min = _chi2
            offset_min = offset
            _scale_min = _scale 

    print 'Found minimum ', offset_min
    # offset_min = 9 # force value
    h_best_fit = h_new_offsets[offset_min]
    h_best_fit.Scale(_scales[offset_min])
    h_best_fit.SetTitle('{0} - {1} days (x {2:1.2f})'.format(_new_country, offset_min, _scales[offset_min]))
    h_best_fit.SetLineStyle(ROOT.kDashed)

    # plot trend for each country + offset curve
    c = ROOT.TCanvas()
    # c.SetLogy()
    h_ref.GetXaxis().SetRangeUser(_x_min_for_plotting, tree.GetEntries() - 1)
    h_ref.SetLineColor(2)
    h_ref.SetMarkerColor(2)
    h_ref.Draw('HISTE')
    h_new.Draw('sameHISTE')
    h_best_fit.Draw('sameHISTE')
    leg = ROOT.TLegend(c.GetLeftMargin(), 0.7, 0.6, 1 - c.GetTopMargin())
    leg.AddEntry(h_ref, _ref_country, 'l')
    leg.AddEntry(h_new, _new_country, 'l')
    leg.AddEntry(h_best_fit, h_best_fit.GetTitle(), 'l')
    # leg.SetFillColor(0)
    leg.SetBorderSize(0)
    leg.Draw()

    # _exp = ROOT.TF1("fit_ref", '[0] * (1  + TMath::Exp(-1 * [1] * (x - [2])))', 0, 51)
    # _exp.SetParNames('L', 'x_0', 'k')
    # h_ref.Fit(_exp)
    # _exp.Draw('same')

    # _exp_f = ROOT.TF1("fit_new", '[0] * (1  + TMath::Exp(-1 * [1] * (x - [2])))', 0, 51)
    # _exp_f.SetParNames('L', 'x_0', 'k')
    # h_new.Fit(_exp_f)
    # _exp_f.Draw('same')

    c.RedrawAxis()
    c.Update()

    c2 = ROOT.TCanvas()
    _chi2_hist.Draw('APL')
    c2.Update()


