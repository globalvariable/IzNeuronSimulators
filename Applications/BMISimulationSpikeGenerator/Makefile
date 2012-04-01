prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: BMISimulationSpikeGenerator

BMISimulationSpikeGenerator: BMISimulationSpikeGenerator.c ../../../BlueSpike/Library/Misc/Misc.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialTypes.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/ExpEnviCommandTimeStamp.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/ExpEnviStatus.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/ExpEnviEventTimeStamp.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialControllerGuiRequestTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/MovObjCommandTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/MovObjEventTimeStamp.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../Library/Network/Network.o ../../Library/InjectionCurrentData/InjectionCurrentData.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o ../../Library/SpikeGenData/SpikeGenData.o ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Neuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o Gui.o CurrentPatternView.o ../../Library/Neuron/NeuronTypes.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../../ExperimentControllers/TrialControllers/Library/Combos/TrialTypesCombo.o ../../Library/Graphs/CurrentPatternGraph.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../Library/Combos/NeuronDynamicsCombo.o BuffersView.o BMISimulationSpikeGeneratorRtTask.o ../../../BlueSpike/Library/Misc/Randn.o ../../Library/SpikeData/SpikeData.o SpikePatternView.o ../../Library/Graphs/SpikePatternGraph.o 
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $<  ../../../BlueSpike/Library/Misc/Misc.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialTypes.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes.o  ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/ExpEnviCommandTimeStamp.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/ExpEnviStatus.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/ExpEnviEventTimeStamp.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialControllerGuiRequestTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/MovObjCommandTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/MovObjEventTimeStamp.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ../../Library/Network/Network.o ../../Library/InjectionCurrentData/InjectionCurrentData.o ../../Library/NeuronDynamicsData/NeuronDynamicsData.o ../../Library/SpikeGenData/SpikeGenData.o  ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Neuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o Gui.o CurrentPatternView.o ../../Library/Neuron/NeuronTypes.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../../ExperimentControllers/TrialControllers/Library/Combos/TrialTypesCombo.o ../../Library/Graphs/CurrentPatternGraph.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../Library/Combos/NeuronDynamicsCombo.o BuffersView.o BMISimulationSpikeGeneratorRtTask.o ../../../BlueSpike/Library/Misc/Randn.o ../../Library/SpikeData/SpikeData.o SpikePatternView.o ../../Library/Graphs/SpikePatternGraph.o ### BMISimulationSpikeGenerator

BMISimulationSpikeGeneratorRtTask.o: BMISimulationSpikeGeneratorRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BMISimulationSpikeGeneratorRtTask

Gui.o: Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

CurrentPatternView.o: CurrentPatternView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### CurrentPatternView

BuffersView.o: BuffersView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BuffersView

SpikePatternView.o: SpikePatternView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### BuffersView

../../Library/Graphs/SpikePatternGraph.o: ../../Library/Graphs/SpikePatternGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### SpikePatternGraph

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

../../../BlueSpike/Library/Misc/Randn.o: ../../../BlueSpike/Library/Misc/Randn.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Randn

../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.o: ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData

../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialTypes.o: ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialTypes

../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes.o: ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes

../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialControllerGuiRequestTimeStamp.o: ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialControllerGuiRequestTimeStamp.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes

../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/ExpEnviCommandTimeStamp.o: ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/ExpEnviCommandTimeStamp.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/ExpEnviCommandTimeStamp

../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/ExpEnviEventTimeStamp.o: ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/ExpEnviEventTimeStamp.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/ExpEnviEventTimeStamp

../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/ExpEnviStatus.o: ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/ExpEnviStatus.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/ExpEnviStatus

../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/MovObjCommandTimeStamp.o: ../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/MovObjCommandTimeStamp.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/MovObjCommandTimeStamp

../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/MovObjEventTimeStamp.o: ../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/MovObjEventTimeStamp.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/MovObjEventTimeStamp

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

../../Library/Synapse/Synapse.o: ../../Library/Synapse/Synapse.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Synapse/Synapse

../../Library/ParkerSochacki/ParkerSochacki.o: ../../Library/ParkerSochacki/ParkerSochacki.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ParkerSochacki/ParkerSochacki

../../Library/InjectionCurrentData/InjectionCurrentData.o: ../../Library/InjectionCurrentData/InjectionCurrentData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/InjectionCurrentData/InjectionCurrentData

../../Library/NeuronDynamicsData/NeuronDynamicsData.o: ../../Library/NeuronDynamicsData/NeuronDynamicsData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/InjectionCurrentData/InjectionCurrentData

../../Library/SpikeGenData/SpikeGenData.o: ../../Library/SpikeGenData/SpikeGenData.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/SpikeGenData/SpikeGenData

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

../../../ExperimentControllers/TrialControllers/Library/Combos/TrialTypesCombo.o: ../../../ExperimentControllers/TrialControllers/Library/Combos/TrialTypesCombo.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../../ExperimentControllers/TrialControllers/Library/Combos/TrialTypesCombo

clean:
	rm -f BMISimulationSpikeGenerator *.o  ../../../BlueSpike/Library/Misc/*.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/*.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/*.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/*.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/*.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/*.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/*.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/*.o ../../../BlueSpike/Library/RtTaskStats/*.o ../../Library/InjectionCurrentData/*.o ../../Library/NeuronDynamicsData/*.o ../../Library/Network/*.o ../../Library/SpikeGenData/*.o ../../Library/Event/*.o ../../Library/Synapse/*.o ../../Library/Neuron/*.o ../../Library/ParkerSochacki/*.o ../../Library/Neuron/*.o ../../Library/Combos/*.o ../../../ExperimentControllers/TrialControllers/Library/Combos/*.o ../../Library/Graphs/*.o ../../Library/SpikeData/*.o ../../Library/Graphs/*.o 

.PHONY: clean

