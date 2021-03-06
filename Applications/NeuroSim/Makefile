prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: NeuroSim

NeuroSim: NeuroSim.c Gui.o SimulationTab.o TrainingTab.o NeuroSimNetwork.o ../Library/Network/Network.o ../Library/ParkerSochacki/ParkerSochacki.o ../Library/Synapse/Synapse.o ../Library/Event/Event.o ../Library/IzNeuron/NeuronTypes.o ../Library/IzNeuron/Neuron.o ../Library/IzNeuronDynamics/NeuronDynamicsPatterns.o ../Library/IzNeuronDynamics/NeuronDynamicsGraph.o ../Library/IzNeuronDynamics/NeuronDynamicsCombo.o ../../BlueSpike/Library/Misc/Misc.o ../Library/ExtNeuron/ExtNeuron.o ../Library/ExtNetwork/ExtNetwork.o ../../SpikePatternGenerator/DataHandling/DataHandling.o ../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.o ../../SpikePatternGenerator/Pattern/Pattern.o ../../BlueSpike/Library/SpikePatterns/SpikePatterns.o NeuroSimSpikePatterns.o ../Library/TrialStats/TrialStats.o NeuroSimTrial.o NeuroSimNeuronDynamics.o ../Library/Combos/LayerNrnGrpNeuronCombo.o NeuroSimSpikePatternGeneratorData.o
	$(CC) $(LXRT_CFLAGS) -o $@ $< Gui.o SimulationTab.o TrainingTab.o NeuroSimNetwork.o ../Library/Network/Network.o ../Library/ParkerSochacki/ParkerSochacki.o ../Library/Synapse/Synapse.o ../Library/Event/Event.o ../Library/IzNeuron/NeuronTypes.o ../Library/IzNeuron/Neuron.o ../Library/IzNeuronDynamics/NeuronDynamicsPatterns.o ../Library/IzNeuronDynamics/NeuronDynamicsGraph.o ../Library/IzNeuronDynamics/NeuronDynamicsCombo.o ../../BlueSpike/Library/Misc/Misc.o ../Library/ExtNeuron/ExtNeuron.o ../Library/ExtNetwork/ExtNetwork.o ../../SpikePatternGenerator/DataHandling/DataHandling.o ../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.o ../../SpikePatternGenerator/Pattern/Pattern.o  ../../BlueSpike/Library/SpikePatterns/SpikePatterns.o NeuroSimSpikePatterns.o ../Library/TrialStats/TrialStats.o NeuroSimTrial.o NeuroSimNeuronDynamics.o ../Library/Combos/LayerNrnGrpNeuronCombo.o NeuroSimSpikePatternGeneratorData.o $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) 
	
Gui.o: Gui.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	

SimulationTab.o: SimulationTab.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

TrainingTab.o: TrainingTab.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

NeuroSimNetwork.o: NeuroSimNetwork.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
		
NeuroSimSpikePatterns.o: NeuroSimSpikePatterns.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	

NeuroSimNeuronDynamics.o: NeuroSimNeuronDynamics.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 
	
NeuroSimSpikePatternGeneratorData.o: NeuroSimSpikePatternGeneratorData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
	
NeuroSimTrial.o: NeuroSimTrial.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
		
../Library/Network/Network.o: ../Library/Network/Network.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

../Library/ParkerSochacki/ParkerSochacki.o: ../Library/ParkerSochacki/ParkerSochacki.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
	
../Library/Synapse/Synapse.o: ../Library/Synapse/Synapse.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
	
../Library/Event/Event.o: ../Library/Event/Event.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

../Library/IzNeuron/NeuronTypes.o: ../Library/IzNeuron/NeuronTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../Library/IzNeuron/Neuron.o: ../Library/IzNeuron/Neuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
	
../Library/ExtNeuron/ExtNeuron.o: ../Library/ExtNeuron/ExtNeuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../Library/ExtNetwork/ExtNetwork.o: ../Library/ExtNetwork/ExtNetwork.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	

../Library/IzNeuronDynamics/NeuronDynamicsPatterns.o: ../Library/IzNeuronDynamics/NeuronDynamicsPatterns.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 			
	
../Library/IzNeuronDynamics/NeuronDynamicsGraph.o: ../Library/IzNeuronDynamics/NeuronDynamicsGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../Library/IzNeuronDynamics/NeuronDynamicsCombo.o: ../Library/IzNeuronDynamics/NeuronDynamicsCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../Library/Combos/LayerNrnGrpNeuronCombo.o: ../Library/Combos/LayerNrnGrpNeuronCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 				

../../BlueSpike/Library/Misc/Misc.o: ../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../../BlueSpike/Library/SpikePatterns/SpikePatterns.o: ../../BlueSpike/Library/SpikePatterns/SpikePatterns.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

../../SpikePatternGenerator/DataHandling/DataHandling.o: ../../SpikePatternGenerator/DataHandling/DataHandling.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
	
../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.o: ../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../../SpikePatternGenerator/Pattern/Pattern.o: ../../SpikePatternGenerator/Pattern/Pattern.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	

../Library/TrialStats/TrialStats.o: ../Library/TrialStats/TrialStats.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	 
	
			
 		# to see warnings
		
clean:
	rm -f *.o NeuroSim ../Library/Network/*.o ../Library/ParkerSochacki/*.o ../Library/Synapse/*.o ../Library/Event/*.o ../Library/IzNeuron/*.o ../Library/IzNeuronDynamics/*.o ../../BlueSpike/Library/Misc/*.o ../Library/ExtNetwork/*.o ../Library/ExtNeuron/*.o ../../SpikePatternGenerator/*.o ../../SpikePatternGenerator/DataHandling/v0/*.o ../../SpikePatternGenerator/Pattern/*.o ../../BlueSpike/Library/SpikePatterns/*.o ../Library/TrialStats/*.o ../Library/Combos/*.o

.PHONY: clean

