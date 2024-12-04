// cells that fire together wire together
//update hebbian link based on new truth value based on old truth value 
//truth value confidence or strength 
//how much infuluce of old truth value will retian truth value will infulence connecion
/**0.1 90% new 10% old 
tcdecayrate how much old value persist
tc varible to store the mean(truthvalue)
old_tc old truth value
new_tc new truth value 

in_envron same enviromnet as current one**/
/*
 ECAN Tutorial

Link one of basic structures used in atomspace used to define hypergraph .

they link two nodes it two nodes are the vertex in each node 
link together with nodes called atoms

2 basic types atom 

nodes is vertex and edge represent relationship or connection 

1  Tree representation structural view 
  A WHERE is root of tree and two branches point to the Nodes "vertex A" b

LINK 1 root "A"  "b"
2 Edge represention (graphical)
 link represent edge connecting two endpoints "A" --"b"--"c" 
Link
   Node "vertex A"
   Node "vertex B"
 

Link
   Node "vertex B"
   Node "vertex C"

universally unique  vertex b same object not two separate entitites consistency and eliminates redundancy 

Unique Links:

Similarly, identical Links (same type and same content) are also unique. If two Links are created with the same Nodes in the same order, they are treated as a reference to the same Link.






outgoing set content of that link
incoming set link all other links that contain it 


Link :"family"
incoming : parent of alice

outgoing set is content of that link  smart pointer
incoming set link set of all other links that contain it 



The MindAgents in this directory carry out the functions required for attention
allocation among atoms within OpenCog


 Atoms
  - HebbianLinks - which 2 atoms is important in same time 
   importance updateing agent collect rent and wages in form of sti and lti
 
   forgetting agent forget with low lti
   hebbianUpdatingagent updates the weights of Hebbian link based on what is in attentional focus of on the opencog at the time it run 




two ways two spread importance deffusion agent and importance spreading agent (spreads sti in Hebbian link  and simple impr)

2014 there is simple importance diffucsion agent 



flow
Rewarding usefull atoms

1 atoms get reward in mindagent if they are usefull in achieving there mindagetn goal

2 stimulus convert into sti and lti by importance updatingagent 
this is the currency collect rent,wages

stimulus   they make use of them best 
it takes this stimulus and pays of to atom 

atoms have to pay lti rent to exist in atomspace 
and sti rent to exist in attentional focus 

things you remember things you spenttime think of give more wages


sti rent charged from atom above attentional focus boundart 
lti rent for all system 

sti =0 all atoms with pos sti are in attentional focus


3 sti spread between hebbianlinks spread by importancediffusionagent and importancespreading agent


The SimpleImportanceDiffusionAgent causes importance diffusion to occur between
atoms in the attentional focus. The main new functionality is the ability to spread importance directly
between non-hebbian incident atoms, that is, atoms in the incoming and outgoing 
sets of a source atom. This property is important for diffusion in a 
generalized hypergraph when importance is allowed to diffuse without relying on
hebbian links as the only diffusion mechanism


atomspace and attention  balance sti and lti 


importance spreading agent 


takes amount of excess importance and spread and spread among Hebbian links

more importance with more Truth value 
amount sti above importance spreading threshold 

importance diffusion agent 
 
diffusive process  markove matrix 












4 hebbianupdatingagent update hebbianlink truthvalue wheahter linked atoms in attentionalvalue or not 
symmetric Hebbian link unorderd
asymmetric Hebbian link  orderd
inverse Hebbian link  orderd 

hebbianlinks weights updated by hebbianUpdating agent 

5 forgetagent remove below threshold lti and  isn't near any other important atoms.
The latter condition is because the ImportanceSpreadingAgent
Forgetting can be tuned via two parameters:

   1. the maximum LTI value that can be forgotten, and
   2. the percentage of the AtomSpace to forget (typically this would be very low!)

6 hebbianCreationModule create new assymetricHebbianLinks as atoms are added to attentional focus
The HebbianCreationModule creates AsymmetricHebbianLinks between atoms in the
AttentionalFocus for updating by the HebbianUpdatingAgent


.
 */

#include <cxxtest/TestSuite.h>
#include <iostream>
#include <opencog/attentionbank/bank/AttentionBank.h>
#include <opencog/attention/HebbianUpdatingAgent.h>
#include <opencog/atomspace/AtomSpace.h>
#include <opencog/atoms/truthvalue/SimpleTruthValue.h>
#include <opencog/cogserver/server/CogServer.h>
#include <opencog/util/Logger.h>
#include <opencog/util/Config.h>
#include <opencog/cogserver/modules/agents/AgentsModule.h>
#include <opencog/attentionbank/types/atom_types.h>

using namespace opencog;

class HebbianUpdatingAgentUTest : public CxxTest::TestSuite
{
private:
    AtomSpace* as;
    CogServer* cogServer;
    Scheduler* scheduler;
    HebbianUpdatingAgent* hebbianUpdatingAgent;
    AttentionBank* _bank;

public:
    void setUp()
    {   // setup intialize the dependancis and HebbianUpdatingagent added to scheduler
        cogServer = &cogserver();
        config().set("MODULES", "opencog/attention/libattention.so");
        cogServer->loadModules();
        config().set("MODULES", "libagents.so");

        Module* amod = cogServer->getModule("opencog::AgentsModule");
        AgentsModule* agmod = static_cast<AgentsModule*>(amod);
        scheduler = &agmod->get_scheduler();

        as = &cogserver().getAtomSpace();
        hebbianUpdatingAgent = new HebbianUpdatingAgent(*cogServer);

        // Add the HebbianUpdatingAgent to the scheduler
        AbstractFactory<Agent>* hebbianUpdatingFactory = new Factory<HebbianUpdatingAgent, Agent>();
        scheduler->registerAgent(HebbianUpdatingAgent::info().id, hebbianUpdatingFactory);

        std::cout << "compeleted setup " << std::endl;
    }

    void tearDown()
    {
        delete hebbianUpdatingAgent;
        cogServer->stop();
        std::cout << "TearDown compeleted and clean resources ." << std::endl;
    }

    void testHebbianLinkUpdate()
    {
        // Create sample atoms in AtomSpace
        Handle intial = as->add_node(CONCEPT_NODE, "intial");
        Handle node1 = as->add_node(CONCEPT_NODE, "Node1");
        Handle node2 = as->add_node(CONCEPT_NODE, "Node2");

        std::cout << "Atoms created: "  ;
        //  put the truth value 
        double initialConfidence = 0.9;
        intial ->setTruthValue(SimpleTruthValue::createTV(0.1, initialConfidence));
        node1->setTruthValue(SimpleTruthValue::createTV(0.3, initialConfidence));
        node2->setTruthValue(SimpleTruthValue::createTV(0.7, initialConfidence));

        std::cout << "Truth values set: " << intial->to_string() << node1->to_string() << node2->to_string() << std::endl;

        // Create a Hebbian link between source and target nodes
        Handle hebbianLink = as->add_link(ASYMMETRIC_HEBBIAN_LINK, intial, node1);
        hebbianLink->setTruthValue(SimpleTruthValue::createTV(0.01, initialConfidence));
        
        std::cout << "i have createad hebbian_link between intial node1 "<< hebbianLink->to_string() << std::endl;

        // Stimulate the atoms to bring them into the attentional focus
        AttentionBank& ab = attentionbank(as);
        ab.stimulate(intial, 200);
        ab.stimulate(node1, 200);
        ab.stimulate(node2, 100);

        std::cout << "sumulates atoms to add to  attentional focus." << std::endl;

        // Run the HebbianUpdatingAgent to update the Hebbian links
        hebbianUpdatingAgent->run();

        std::cout << "HebbianUpdatingAgent run completed." << std::endl;

        // Retrieve the updated truth value of the Hebbian link
        TruthValuePtr updatedTV = hebbianLink->getTruthValue();
        double updatedMean = updatedTV->get_mean();

        std::cout << "Updated truth value of Hebbian link: " << updatedMean << std::endl;
        std::cout << "Hebbian link updated"<< hebbianLink->to_string() << std::endl;

        // Check that the truth value of the link has changed based on the agent's logic
        // Intentionally made error by making the mean 0.9 so that we can see what's happening

        TS_ASSERT_DELTA(updatedMean, 0.0105976, 0.01); // Expected value within a delta range
    }
};