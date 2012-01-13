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

NeuroSim: NeuroSim.c Gui.o SimulationTab.o TrainingTab.o NeuroSimPattern.o NeuroSimNetwork.o ../Library/Network/Network.o ../Library/ParkerSochacki/ParkerSochacki.o ../Library/Synapse/Synapse.o ../Library/Event/Event.o ../Library/IzNeuron/NeuronTypes.o ../Library/IzNeuron/Neuron.o ../Library/IzNeuron/NeuronDynamicsGraph.o ../Library/IzNeuron/NeuronDynamicsCombo.o ../../BlueSpike/Library/Misc/Misc.o ../Library/ExtNeuron/ExtNeuron.o ../Library/ExtNetwork/ExtNetwork.o ../../SpikePatternGenerator/DataHandling/DataHandling.o ../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.o ../../SpikePatternGenerator/Pattern/Pattern.o 
	$(CC) $(LXRT_CFLAGS) -o $@ $< Gui.o SimulationTab.o TrainingTab.o NeuroSimPattern.o NeuroSimNetwork.o ../Library/Network/Network.o ../Library/ParkerSochacki/ParkerSochacki.o ../Library/Synapse/Synapse.o ../Library/Event/Event.o ../Library/IzNeuron/NeuronTypes.o ../Library/IzNeuron/Neuron.o ../Library/IzNeuron/NeuronDynamicsGraph.o ../Library/IzNeuron/NeuronDynamicsCombo.o ../../BlueSpike/Library/Misc/Misc.o ../Library/ExtNeuron/ExtNeuron.o ../Library/ExtNetwork/ExtNetwork.o ../../SpikePatternGenerator/DataHandling/DataHandling.o ../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.o ../../SpikePatternGenerator/Pattern/Pattern.o  $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) 
	
Gui.o: Gui.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	

SimulationTab.o: SimulationTab.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

TrainingTab.o: TrainingTab.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

NeuroSimPattern.o: NeuroSimPattern.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	

NeuroSimNetwork.o: NeuroSimNetwork.c 
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
	
../Library/IzNeuron/NeuronDynamicsGraph.o: ../Library/IzNeuron/NeuronDynamicsGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../Library/IzNeuron/NeuronDynamicsCombo.o: ../Library/IzNeuron/NeuronDynamicsCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 			

../../BlueSpike/Library/Misc/Misc.o: ../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		

../../SpikePatternGenerator/DataHandling/DataHandling.o: ../../SpikePatternGenerator/DataHandling/DataHandling.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
	
../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.o: ../../SpikePatternGenerator/DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 		
	
../../SpikePatternGenerator/Pattern/Pattern.o: ../../SpikePatternGenerator/Pattern/Pattern.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 	
		
 		# to see warnings
		
clean:
	rm -f *.o NeuroSim ../Library/Network/*.o ../Library/ParkerSochacki/*.o ../Library/Synapse/*.o ../Library/Event/*.o ../Library/IzNeuron/*.o ../Library/IzNeuron/*.o ../../BlueSpike/Library/Misc/*.o ../Library/ExtNetwork/*.o ../Library/ExtNeuron/*.o ../../SpikePatternGenerator/*.o ../../SpikePatternGenerator/DataHandling/v0/*.o ../../SpikePatternGenerator/Pattern/*.o

.PHONY: clean

