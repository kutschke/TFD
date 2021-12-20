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

namespace {

}

namespace mu2e {

  // Base class for a pair of adapter classes to solve the problem with
  // the ternary operator.


  class TestPayload{
  public:
    static int _counter;

    TestPayload mkdir(std::string const& s ){
      return TestPayload(s);
    }

  protected:
    TestPayload( std::string const& a ):_payload(a),_serialNumber(++_counter){
      cout << "TestPayload the c'tor: " << _payload << endl;
    }

  private:
    std::string _payload;
    int _serialNumber;
  };

  class PayloadMaker : public TestPayload{
  public:
    PayloadMaker():TestPayload("foo"){
      cout << "In PayloadMaker c'tor" << endl;
    }
    ~PayloadMaker(){
      cout << "In PayloadMaker d'tor" << endl;
    }
  };

  class AdapterBase{
  public:
    virtual ~AdapterBase(){};
    virtual art::TFileDirectory const& get() = 0;
  };

  // One of the two concrete classes
  class LAdapter : public AdapterBase{
  public:
    LAdapter( art::TFileDirectory& tfdir ):_tfdir(tfdir){
    }
    art::TFileDirectory const& get() { return _tfdir; }
  private:
    art::TFileDirectory& _tfdir;
  };

  // The other of the two concrete classes.
  class RAdapter : public AdapterBase{
  public:
    RAdapter( art::TFileDirectory&& tfdir ):_tfdir(std::move(tfdir)){
    }
    art::TFileDirectory const& get() { return _tfdir; }
  private:
    art::TFileDirectory const&& _tfdir;
  };


  int TestPayload::_counter = 0;

  class TFD4 : public art::EDAnalyzer {

  public:
    explicit TFD4(fhicl::ParameterSet const& pset);

    void analyze(const art::Event& event) override;
    void helper ( art::TFileDirectory & topdir, std::string const& name );

  private:
    TH1F* _h1 = nullptr;
    TH1F* _h2 = nullptr;
    TH1F* _h3 = nullptr;

  };

  void TFD4::helper ( art::TFileDirectory & topdir, std::string const& name ){
    cout << "helper: " << name << endl;

    //art::TFileDirectory  tfdir = name.empty() ? topdir : topdir.mkdir(name.c_str());
    //art::TFileDirectory&& tfdir = topdir;
    //art::TFileDirectory const&  tfdir  = topdir.mkdir(name.c_str());
    //art::TFileDirectory         tfdir = topdir.mkdir(name.c_str());
    art::TFileDirectory&&         tfdir = topdir.mkdir(name.c_str());

    AdapterBase* a1 = new LAdapter(topdir);
    cout << "a1: " << &a1 << endl;

    AdapterBase* a2 = new RAdapter(topdir.mkdir(name.c_str()));
    cout << "a2: " << &a1 << endl;

    AdapterBase* a3 = name.empty() ?
      static_cast<AdapterBase*>(new LAdapter(topdir)) :
      static_cast<AdapterBase*>(new RAdapter(topdir.mkdir(name.c_str())));
    cout << "a3: " << &a3 << endl;

    if ( ! name.empty() ){
      _h3 = a2->get().make<TH1F>( "h2", "Event Number * 3", 10,  0.,   10. );
    } else{
      _h3 = a1->get().make<TH1F>( "h2", "Event Number * 3", 10,  0.,   10. );

    }

    //art::TFileDirectory  tfdir = name.empty() ? topdir : topdir.mkdir(name.c_str());

    //PayloadMaker mk;
    //cout << "maker: " << &mk << endl;
  }

  TFD4::TFD4(fhicl::ParameterSet const& pset)
    : art::EDAnalyzer(pset){

    art::ServiceHandle<art::TFileService> tfs;
    helper ( *tfs, "foo");
  }

  void TFD4::analyze(const art::Event& event){
    cerr << "Hello, world.  From analyze: "
         << event.id()
         << endl;
    _h1->Fill(    event.id().event() );
    _h2->Fill( 2.*event.id().event() );
  }

} // end namespace mu2e

DEFINE_ART_MODULE(mu2e::TFD4);
