prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: HybridBMI

HybridBMI: HybridBMI.c Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o ../../Library/IzNeuron/NeuronTypes.o ../../Library/IzNeuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Network/Network.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $< Gui.o NetworkView.o ../../../BlueSpike/Library/Misc/Misc.o  ../../Library/IzNeuron/NeuronTypes.o ../../Library/IzNeuron/Neuron.o ../../Library/ParkerSochacki/ParkerSochacki.o ../../Library/Event/Event.o ../../Library/Synapse/Synapse.o ../../Library/Network/Network.o ### HybridBMI

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

../../Library/Event/Event.o: ../../Library/Event/Event.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Event/Event

../../Library/Synapse/Synapse.o: ../../Library/Synapse/Synapse.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/Synapse/Synapse

../../Library/ParkerSochacki/ParkerSochacki.o: ../../Library/ParkerSochacki/ParkerSochacki.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)		###	../../Library/ParkerSochacki/ParkerSochacki

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

clean:
	rm -f HybridBMI *.o ../../../BlueSpike/Library/Misc/*.o ../../Library/IzNeuron/*.o  ../../Library/ParkerSochacki/*.o ../../Library/Event/*.o ../../Library/Synapse/*.o ../../Library/Network/*.o

.PHONY: clean

