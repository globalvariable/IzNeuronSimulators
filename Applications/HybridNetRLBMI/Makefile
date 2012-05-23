prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: HybridNetRLBMI

HybridNetRLBMI: HybridNetRLBMI.c HybridNetRLBMIRtTask.o Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o ../../Library/Neuron/NeuronTypes.o ../../Library/Neuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Network/Network.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../../BlueSpike/Library/Misc/Randn.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o BufferViewHandler.o NeuronDynamicsView.o ../../Library/SpikeData/SpikeData.o BlueSpikeSpikePatternView.o InSilicoSpikePatternView.o ../../Library/Graphs/StatusMarker.o ../../Library/MotorOutput/MotorOutput.o ../../Library/MotorOutput/MotorOutputTypes.o ../../Library/MotorOutput/MotorOutputTimeStamp.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< HybridNetRLBMIRtTask.o Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o  ../../Library/Neuron/NeuronTypes.o ../../Library/Neuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Network/Network.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../../BlueSpike/Library/Misc/Randn.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o BufferViewHandler.o NeuronDynamicsView.o ../../Library/SpikeData/SpikeData.o BlueSpikeSpikePatternView.o InSilicoSpikePatternView.o ../../Library/Graphs/StatusMarker.o ../../Library/MotorOutput/MotorOutput.o ../../Library/MotorOutput/MotorOutputTypes.o ../../Library/MotorOutput/MotorOutputTimeStamp.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o ### HybridNetRLBMI

HybridNetRLBMIRtTask.o: HybridNetRLBMIRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

Gui.o: Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

NetworkView.o: NetworkView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### NetworkView

BufferViewHandler.o: BufferViewHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BufferViewHandler

NeuronDynamicsView.o: NeuronDynamicsView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### NeuronDynamicsView

BlueSpikeSpikePatternView.o: BlueSpikeSpikePatternView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BlueSpikeSpikePatternView

InSilicoSpikePatternView.o: InSilicoSpikePatternView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BlueSpikeSpikePatternView

../../Library/Neuron/NeuronTypes.o: ../../Library/Neuron/NeuronTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Neuron/NeuronTypes

../../Library/Neuron/Neuron.o: ../../Library/Neuron/Neuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Neuron/Neuron

../../Library/Network/Network.o: ../../Library/Network/Network.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Network/Network

../../Library/Combos/LayerNrnGrpNeuronCombo.o: ../../Library/Combos/LayerNrnGrpNeuronCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Combos/LayerNrnGrpNeuronCombo

../../Library/Event/Event.o: ../../Library/Event/Event.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Event/Event

../../Library/Synapse/Synapse.o: ../../Library/Synapse/Synapse.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Synapse/Synapse

../../Library/ParkerSochacki/ParkerSochacki.o: ../../Library/ParkerSochacki/ParkerSochacki.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ParkerSochacki/ParkerSochacki

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../BlueSpike/Library/RtTaskStats/RtTaskStats

../../Library/Combos/NeuronDynamicsCombo.o: ../../Library/Combos/NeuronDynamicsCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/Combos/NeuronDynamicsCombo

##../../Library/Graphs/SpikePatternGraph.o: ../../Library/Graphs/SpikePatternGraph.c
##	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/SpikePatternGraph

##../../Library/Graphs/NeuronDynamicsGraph.o: ../../Library/Graphs/NeuronDynamicsGraph.c
##	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/NeuronDynamicsGraph

../../Library/Graphs/StatusMarker.o: ../../Library/Graphs/StatusMarker.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/StatusMarker

../../Library/NeuronDynamicsData/NeuronDynamicsData.o: ../../Library/NeuronDynamicsData/NeuronDynamicsData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/NeuronDynamicsData/NeuronDynamicsData

##../../Library/InjectionCurrentData/InjectionCurrentData.o: ../../Library/InjectionCurrentData/InjectionCurrentData.c
##	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/InjectionCurrentData/InjectionCurrentData

../../../BlueSpike/Library/Misc/Randn.o: ../../../BlueSpike/Library/Misc/Randn.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Randn

../../Library/SpikeData/SpikeData.o: ../../Library/SpikeData/SpikeData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/SpikeData/SpikeData

../../Library/MotorOutput/MotorOutput.o: ../../Library/MotorOutput/MotorOutput.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/MotorOutput/MotorOutput

../../Library/MotorOutput/MotorOutputTypes.o: ../../Library/MotorOutput/MotorOutputTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/MotorOutput/MotorOutputTypes

../../Library/MotorOutput/MotorOutputTimeStamp.o: ../../Library/MotorOutput/MotorOutputTimeStamp.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/MotorOutput/MotorOutputTimeStamp

../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet

../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand

../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet

../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand

clean:
	rm -f HybridNetRLBMI *.o ../../../BlueSpike/Library/Misc/*.o ../../Library/Neuron/*.o  ../../Library/ParkerSochacki/*.o ../../Library/Event/*.o ../../Library/Synapse/*.o ../../Library/Network/*.o ../../Library/Combos/*.o ../../Library/Graphs/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../Library/InjectionCurrentData/*.o ../../Library/NeuronDynamicsData/*.o ../../Library/SpikeData/*.o ../../Library/MotorOutput/*.o ../../../ExperimentHandlers/Library/Messages/*.o

.PHONY: clean

