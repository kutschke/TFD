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

  class TFD1 : public art::EDAnalyzer {

  public:
    explicit TFD1(fhicl::ParameterSet const& pset);

    void analyze(const art::Event& event);

  private:
    TH1F* _h1 = nullptr;
    TH1F* _h2 = nullptr;


  };

  TFD1::TFD1(fhicl::ParameterSet const& pset)
    : art::EDAnalyzer(pset){

    art::ServiceHandle<art::TFileService> tfs;
    _h1 = tfs->make<TH1F>( "h1", "Event Number", 10,  0.,   10. );

    art::TFileDirectory&& tfdir{tfs->mkdir("foo")};
    _h2 = tfdir.make<TH1F>( "h2", "Event Number * 2", 10,  0.,   10. );
  }

  void TFD1::analyze(const art::Event& event){
    cerr << "Hello, world.  From analyze: "
         << event.id()
         << endl;
    _h1->Fill(    event.id().event() );
    _h2->Fill( 2.*event.id().event() );
  }

} // end namespace mu2e

DEFINE_ART_MODULE(mu2e::TFD1);
