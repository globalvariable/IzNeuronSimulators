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

HybridNetRLBMI: HybridNetRLBMI.c HybridNetRLBMIRtTask.o Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o ../../Library/IzNeuron/NeuronTypes.o ../../Library/IzNeuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Network/Network.o ../../Library/ExtNetwork/ExtNetwork.o ../../Library/ExtNeuron/ExtNeuron.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< HybridNetRLBMIRtTask.o Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o  ../../Library/IzNeuron/NeuronTypes.o ../../Library/IzNeuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Network/Network.o ../../Library/ExtNetwork/ExtNetwork.o ../../Library/ExtNeuron/ExtNeuron.o ../../Library/Combos/LayerNrnGrpNeuronCombo.o ../../Library/Combos/NeuronDynamicsCombo.o ../../Library/Graphs/NeuronDynamicsGraph.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ### HybridNetRLBMI

HybridNetRLBMIRtTask.o: HybridNetRLBMIRtTask.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

Gui.o: Gui.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### Gui

NetworkView.o: NetworkView.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### NetworkView

../../Library/IzNeuron/NeuronTypes.o: ../../Library/IzNeuron/NeuronTypes.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/IzNeuron/NeuronTypes

../../Library/IzNeuron/Neuron.o: ../../Library/IzNeuron/Neuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/IzNeuron/Neuron

../../Library/Network/Network.o: ../../Library/Network/Network.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Network/Network

../../Library/ExtNetwork/ExtNetwork.o: ../../Library/ExtNetwork/ExtNetwork.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ExtNetwork/ExtNetwork

../../Library/ExtNeuron/ExtNeuron.o: ../../Library/ExtNeuron/ExtNeuron.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ExtNeuron/ExtNeuron

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

../../Library/Graphs/NeuronDynamicsGraph.o: ../../Library/Graphs/NeuronDynamicsGraph.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Graphs/IzNeuronDynamicsGraph

clean:
	rm -f HybridNetRLBMI *.o ../../../BlueSpike/Library/Misc/*.o ../../Library/IzNeuron/*.o  ../../Library/ParkerSochacki/*.o ../../Library/Event/*.o ../../Library/Synapse/*.o ../../Library/Network/*.o ../../Library/ExtNetwork/*.o ../../Library/ExtNeuron/*.o ../../Library/Combos/*.o ../../Library/Graphs/*.o ../../../BlueSpike/Library/RtTaskStats/*.o

.PHONY: clean

