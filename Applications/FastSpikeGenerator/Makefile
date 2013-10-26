prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: BMISpikeGenerator

BMISpikeGenerator: BMISpikeGenerator.c ../../Library/Network/Network.o ../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen.o ../../../ExperimentHandlers/Library/Messages/SpikeGen2TrialHand.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../../BlueSpike/Library/Misc/Misc.o Gui.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../Library/Neuron/NeuronTypes.o ../../Library/Neuron/Neuron.o ../../Library/Event/Event.o ../../Library/Axon/Axon.o ../../Library/ParkerSochacki/ParkerSochacki.o CurrentPatternDesignView.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o  BMISpikeGeneratorRtTask.o ../../Library/Graphs/CurrentPatternGraph.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../Library/InjectionCurrentData/InjectionCurrentData.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o ../../Library/SpikeData/SpikeData.o BufferViewHandler.o NeuronDynamicsCurrentBufferView.o ../../../BlueSpike/Library/SpikeGen/SpikeGen.o ../../Library/Graphs/StatusMarker.o ../../../BlueSpike/Library/Misc/Randn.o SpikePatternBufferView.o ../../Library/Graphs/SpikePatternGraph.o ../../Library/Eligibility/Eligibility.o FileHandler.o FileHandler_v0.o ../../Library/ParkerSochacki/ParkerSochackiSynapseCond.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $<  ../../Library/Network/Network.o ../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen.o ../../../ExperimentHandlers/Library/Messages/SpikeGen2TrialHand.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../../BlueSpike/Library/Misc/Misc.o Gui.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../Library/Neuron/NeuronTypes.o ../../Library/Neuron/Neuron.o ../../Library/Event/Event.o ../../Library/Axon/Axon.o ../../Library/ParkerSochacki/ParkerSochacki.o CurrentPatternDesignView.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o BMISpikeGeneratorRtTask.o ../../Library/Graphs/CurrentPatternGraph.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../Library/InjectionCurrentData/InjectionCurrentData.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o ../../Library/SpikeData/SpikeData.o BufferViewHandler.o NeuronDynamicsCurrentBufferView.o ../../../BlueSpike/Library/SpikeGen/SpikeGen.o ../../Library/Graphs/StatusMarker.o ../../../BlueSpike/Library/Misc/Randn.o SpikePatternBufferView.o ../../Library/Graphs/SpikePatternGraph.o ../../Library/Eligibility/Eligibility.o FileHandler.o FileHandler_v0.o ../../Library/ParkerSochacki/ParkerSochackiSynapseCond.o ### BMISpikeGenerator

BMISpikeGeneratorRtTask.o: BMISpikeGeneratorRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BMISpikeGeneratorRtTask

Gui.o: Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

CurrentPatternDesignView.o: CurrentPatternDesignView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### CurrentPatternDesignView

BufferViewHandler.o: BufferViewHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BufferViewHandler

NeuronDynamicsCurrentBufferView.o: NeuronDynamicsCurrentBufferView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### NeuronDynamicsCurrentBufferView

SpikePatternBufferView.o: SpikePatternBufferView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SpikePatternBufferView

../../Library/Graphs/SpikePatternGraph.o: ../../Library/Graphs/SpikePatternGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/Graphs/SpikePatternGraph

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

../../../BlueSpike/Library/Misc/Randn.o: ../../../BlueSpike/Library/Misc/Randn.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Randn

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../BlueSpike/Library/RtTaskStats/RtTaskStats

../../Library/Network/Network.o: ../../Library/Network/Network.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Network/Network

../../Library/Neuron/NeuronTypes.o: ../../Library/Neuron/NeuronTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Neuron/NeuronTypes

../../Library/Neuron/Neuron.o: ../../Library/Neuron/Neuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Neuron/Neuron

../../Library/Event/Event.o: ../../Library/Event/Event.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Event/Event

../../Library/Axon/Axon.o: ../../Library/Axon/Axon.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Axon/Axon

../../Library/ParkerSochacki/ParkerSochacki.o: ../../Library/ParkerSochacki/ParkerSochacki.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ParkerSochacki/ParkerSochacki

../../Library/ParkerSochacki/ParkerSochackiSynapseCond.o: ../../Library/ParkerSochacki/ParkerSochackiSynapseCond.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ParkerSochacki/ParkerSochackiSynapseCond

../../Library/InjectionCurrentData/InjectionCurrentData.o: ../../Library/InjectionCurrentData/InjectionCurrentData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/InjectionCurrentData/InjectionCurrentData

../../Library/NeuronDynamicsData/NeuronDynamicsData.o: ../../Library/NeuronDynamicsData/NeuronDynamicsData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/InjectionCurrentData/InjectionCurrentData

../../Library/SpikeData/SpikeData.o: ../../Library/SpikeData/SpikeData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/SpikeData/SpikeData

../../Library/Combos/LayerNrnGrpNeuronCombo.o: ../../Library/Combos/LayerNrnGrpNeuronCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Combos/LayerNrnGrpNeuronCombo

../../Library/Combos/NeuronDynamicsCombo.o: ../../Library/Combos/NeuronDynamicsCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Combos/NeuronDynamicsCombo

../../Library/Graphs/CurrentPatternGraph.o: ../../Library/Graphs/CurrentPatternGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/CurrentPatternGraph

../../Library/Graphs/NeuronDynamicsGraph.o: ../../Library/Graphs/NeuronDynamicsGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/NeuronDynamicsGraph

../../Library/Graphs/StatusMarker.o: ../../Library/Graphs/StatusMarker.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/StatusMarker

../../Library/Eligibility/Eligibility.o: ../../Library/Eligibility/Eligibility.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Eligibility/Eligibility

../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentHandlers/Library/Messages/TrialHand2SpikeGen

../../../ExperimentHandlers/Library/Messages/SpikeGen2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/SpikeGen2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentHandlers/Library/Messages/SpikeGen2TrialHand

../../../ExperimentHandlers/Library/Status/TrialStatus.o: ../../../ExperimentHandlers/Library/Status/TrialStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentHandlers/Library/Status/TrialStatus

../../../BlueSpike/Library/SpikeGen/SpikeGen.o: ../../../BlueSpike/Library/SpikeGen/SpikeGen.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../BlueSpike/Library/SpikeGen/SpikeGen

FileHandler.o: FileHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### FileHandler

FileHandler_v0.o: FileHandler_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### FileHandler_v0

clean:
	rm -f BMISpikeGenerator *.o  ../../Library/Network/*.o ../../../ExperimentHandlers/Library/Messages/*.o ../../../ExperimentHandlers/Library/Status/*.o ../../../BlueSpike/Library/Misc/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../Library/Neuron/*.o ../../Library/Neuron/*.o ../../Library/Event/*.o ../../Library/Axon/*.o ../../Library/ParkerSochacki/*.o ../../Library/Combos/*.o ../../../ExperimentHandlers/Library/Combos/*.o ../../Library/Graphs/*.o ../../Library/InjectionCurrentData/*.o ../../Library/NeuronDynamicsData/*.o ../../Library/SpikeData/*.o ../../../BlueSpike/Library/SpikeGen/*.o ../../Library/Eligibility/*.o

.PHONY: clean

