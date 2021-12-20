//
//  Test usage of TFileService mkdir in art v3.10
//
//  Original author Rob Kutschke
//

#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art_root_io/TFileService.h"

#include "TH1F.h"

#include <iostream>

using namespace std;

namespace mu2e {

  class TFD5 : public art::EDAnalyzer {
  public:
    explicit TFD5(fhicl::ParameterSet const& pset);

    void analyze(const art::Event& event) override;

  private:
    TH1F* _h1 = nullptr;
    int   _n  = 1;

  };

  TFD5::TFD5(fhicl::ParameterSet const& pset)
    : art::EDAnalyzer(pset),
      _n(pset.get<int>("number")){

    art::ServiceHandle<art::TFileService> tfs;

    art::TFileDirectory tfdir = tfs->mkdir(pset.get<string>("directoryName"));
    _h1 = tfdir.make<TH1F>( "h1", "(Event Number) * Number", 10,  0.,   10. );

  }

  void TFD5::analyze(const art::Event& event){
    _h1->Fill( event.id().event()*_n );
  }

} // end namespace mu2e

DEFINE_ART_MODULE(mu2e::TFD5);
