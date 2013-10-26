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

HybridNetRLBMI: HybridNetRLBMI.c HybridNetRLBMIRtTask.o Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o ../../Library/Neuron/NeuronTypes.o ../../Library/Neuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Axon/Axon.o ../../Library/Network/Network.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../../BlueSpike/Library/Misc/Randn.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o BufferViewHandler.o NeuronDynamicsView.o ../../Library/SpikeData/SpikeData.o BlueSpikeSpikePatternView.o InSilicoSpikePatternView.o ../../Library/Graphs/StatusMarker.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../Library/Graphs/SpikePatternGraph.o ../../Library/STDP/STDP.o ../../Library/Eligibility/Eligibility.o ../../Library/STDPData/STDPData.o ../../Library/Graphs/STDPGraph.o ../../Library/Combos/STDPCombo.o ../../Library/Synapse/Synapse.o ../../Library/Combos/LayerNrnGrpNrnSynapseCombo.o STDPView.o ../../Library/EligibilityData/EligibilityData.o EligibilityView.o ../../Library/Graphs/EligibilityGraph.o ../../Library/Stats/FiringStats.o ../../Library/Graphs/SynapticWeightHistGraph.o WeightHistoryView.o ../../Library/InjectionCurrentData/InjectionCurrentData.o ../../Library/Spindle/Spindle.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o HybridNetRLBMIConfig.o ../../Library/ParkerSochacki/PSPrePostSTDPElig.o AxonToLayerWeightView.o ../../Library/Neuron/PoissonNeuron.o 
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< HybridNetRLBMIRtTask.o Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o  ../../Library/Neuron/NeuronTypes.o ../../Library/Neuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Axon/Axon.o ../../Library/Network/Network.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../../BlueSpike/Library/Misc/Randn.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o BufferViewHandler.o NeuronDynamicsView.o ../../Library/SpikeData/SpikeData.o BlueSpikeSpikePatternView.o InSilicoSpikePatternView.o ../../Library/Graphs/StatusMarker.o ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o ../../../ExperimentHandlers/Library/Status/TrialStatus.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../Library/Graphs/SpikePatternGraph.o ../../Library/STDP/STDP.o ../../Library/Eligibility/Eligibility.o ../../Library/STDPData/STDPData.o ../../Library/Graphs/STDPGraph.o ../../Library/Combos/STDPCombo.o ../../Library/Synapse/Synapse.o ../../Library/Combos/LayerNrnGrpNrnSynapseCombo.o STDPView.o ../../Library/EligibilityData/EligibilityData.o EligibilityView.o ../../Library/Graphs/EligibilityGraph.o ../../Library/Stats/FiringStats.o ../../Library/Graphs/SynapticWeightHistGraph.o WeightHistoryView.o ../../Library/InjectionCurrentData/InjectionCurrentData.o ../../Library/Spindle/Spindle.o ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.o DataHandling/DataHandling.o DataHandling/v0/DataFormat_v0.o HybridNetRLBMIConfig.o ../../Library/ParkerSochacki/PSPrePostSTDPElig.o AxonToLayerWeightView.o ../../Library/Neuron/PoissonNeuron.o ### HybridNetRLBMI

HybridNetRLBMIConfig.o: HybridNetRLBMIConfig.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HybridNetRLBMIConfig

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

STDPView.o: STDPView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### STDPView

EligibilityView.o: EligibilityView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### EligibilityView

WeightHistoryView.o: WeightHistoryView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### WeightHistoryView

AxonToLayerWeightView.o: AxonToLayerWeightView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### AxonToLayerWeightView

HandleNeuralNet2NeuralNetSimMsgs.o: HandleNeuralNet2NeuralNetSimMsgs.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### HandleNeuralNet2NeuralNetSimMsgs

../../Library/Neuron/NeuronTypes.o: ../../Library/Neuron/NeuronTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Neuron/NeuronTypes

../../Library/Neuron/Neuron.o: ../../Library/Neuron/Neuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Neuron/Neuron

../../Library/Network/Network.o: ../../Library/Network/Network.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Network/Network

../../Library/InjectionCurrentData/InjectionCurrentData.o: ../../Library/InjectionCurrentData/InjectionCurrentData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/InjectionCurrentData/InjectionCurrentData

../../Library/Combos/LayerNrnGrpNeuronCombo.o: ../../Library/Combos/LayerNrnGrpNeuronCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Combos/LayerNrnGrpNeuronCombo

../../Library/Combos/LayerNrnGrpNrnSynapseCombo.o: ../../Library/Combos/LayerNrnGrpNrnSynapseCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Combos/LayerNrnGrpNrnSynapseCombo

../../Library/Event/Event.o: ../../Library/Event/Event.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Event/Event

../../Library/Axon/Axon.o: ../../Library/Axon/Axon.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Axon/Axon

../../Library/Synapse/Synapse.o: ../../Library/Synapse/Synapse.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Synapse/Synapse

../../Library/STDP/STDP.o: ../../Library/STDP/STDP.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/STDP/STDP

../../Library/STDPData/STDPData.o: ../../Library/STDPData/STDPData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/STDPData/STDPData

../../Library/Graphs/STDPGraph.o: ../../Library/Graphs/STDPGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/STDPGraph

../../Library/Combos/STDPCombo.o: ../../Library/Combos/STDPCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Combos/STDPCombo

../../Library/Eligibility/Eligibility.o: ../../Library/Eligibility/Eligibility.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Eligibility/Eligibility

../../Library/EligibilityData/EligibilityData.o: ../../Library/EligibilityData/EligibilityData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/EligibilityData/EligibilityData

../../Library/Graphs/EligibilityGraph.o: ../../Library/Graphs/EligibilityGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/EligibilityGraph

../../Library/ParkerSochacki/ParkerSochacki.o: ../../Library/ParkerSochacki/ParkerSochacki.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ParkerSochacki/ParkerSochacki

../../Library/ParkerSochacki/PSPrePostSTDPElig.o: ../../Library/ParkerSochacki/PSPrePostSTDPElig.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ParkerSochacki/PSPrePostSTDPElig

../../Library/Stats/FiringStats.o: ../../Library/Stats/FiringStats.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Stats/FiringStats

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o: ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../BlueSpike/Library/RtTaskStats/RtTaskStats

../../Library/Combos/NeuronDynamicsCombo.o: ../../Library/Combos/NeuronDynamicsCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/Combos/NeuronDynamicsCombo

../../Library/Graphs/SpikePatternGraph.o: ../../Library/Graphs/SpikePatternGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)  		###	../../Library/Graphs/SpikePatternGraph

../../Library/Graphs/NeuronDynamicsGraph.o: ../../Library/Graphs/NeuronDynamicsGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/NeuronDynamicsGraph

../../Library/Graphs/SynapticWeightHistGraph.o: ../../Library/Graphs/SynapticWeightHistGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/SynapticWeightHistGraph

../../Library/Graphs/StatusMarker.o: ../../Library/Graphs/StatusMarker.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/StatusMarker

../../Library/NeuronDynamicsData/NeuronDynamicsData.o: ../../Library/NeuronDynamicsData/NeuronDynamicsData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/NeuronDynamicsData/NeuronDynamicsData

../../../BlueSpike/Library/Misc/Randn.o: ../../../BlueSpike/Library/Misc/Randn.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Randn

../../Library/SpikeData/SpikeData.o: ../../Library/SpikeData/SpikeData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/SpikeData/SpikeData

../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/TrialHand2NeuralNet

../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2TrialHand

../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.o: ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/MovObjHand2NeuralNet

../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2MovObjHand

../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.o: ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Messages/NeuralNet2Gui

../../../ExperimentHandlers/Library/Status/TrialStatus.o: ../../../ExperimentHandlers/Library/Status/TrialStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../ExperimentHandlers/Library/Status/TrialStatus

../../Library/Spindle/Spindle.o: ../../Library/Spindle/Spindle.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../Library/Spindle/Spindle

DataHandling/DataHandling.o: DataHandling/DataHandling.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/DataHandling

DataHandling/v0/DataFormat_v0.o: DataHandling/v0/DataFormat_v0.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### DataHandling/v0/DataFormat_v0

../../Library/Neuron/PoissonNeuron.o: ../../Library/Neuron/PoissonNeuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Neuron/PoissonNeuron

clean:
	rm -f HybridNetRLBMI *.o ../../../BlueSpike/Library/Misc/*.o ../../Library/Neuron/*.o  ../../Library/ParkerSochacki/*.o ../../Library/Event/*.o ../../Library/Axon/*.o ../../Library/Network/*.o ../../Library/Combos/*.o ../../Library/Graphs/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../Library/InjectionCurrentData/*.o ../../Library/NeuronDynamicsData/*.o ../../Library/SpikeData/*.o ../../../ExperimentHandlers/Library/Messages/*.o ../../../ExperimentHandlers/Library/Status/*.o ../../Library/STDP/*.o ../../Library/Eligibility/*.o ../../Library/STDPData/*.o ../../Library/Synapse/*.o ../../Library/EligibilityData/*.o ../../Library/Stats/*.o ../../Library/InjectionCurrentData/*.o ../../Library/Spindle/*.o DataHandling/*.o DataHandling/v0/*.o

.PHONY: clean

