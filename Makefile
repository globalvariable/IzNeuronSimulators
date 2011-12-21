prefix := $(shell rtai-config --prefix)

ifeq ($(prefix),)
$(error Please add <rtai-install>/bin to your PATH variable)
endif

CC = $(shell rtai-config --cc)
LXRT_CFLAGS = $(shell rtai-config --lxrt-cflags)
LXRT_LDFLAGS = $(shell rtai-config --lxrt-ldflags)
COMEDI_FLAGS = -lcomedi 
MATH_FLAGS = -lm
GTK_FLAGS = $(shell pkg-config --cflags --libs gtk+-2.0 gtkdatabox)

all: NeuroSim

NeuroSim: NeuroSim.c Gui.o RTHandler.o Simulate.o ParkerSochacki.o Network.o Neuron.o Logs.o Graphs.o Notebooks.o
	$(CC) $(LXRT_CFLAGS) -o $@ $<  Gui.o RTHandler.o Simulate.o ParkerSochacki.o Network.o Neuron.o Logs.o Graphs.o Notebooks.o $(LXRT_LDFLAGS) $(COMEDI_FLAGS) $(MATH_FLAGS) $(GTK_FLAGS) 

Gui.o: Gui.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 

RTHandler.o: RTHandler.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS) 

Simulate.o: Simulate.c
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $<  $(GTK_FLAGS) 

Network.o: Network.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(GTK_FLAGS) 

ParkerSochacki.o: ParkerSochacki.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(GTK_FLAGS) 

Notebooks.o: Notebooks.c 
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $< $(GTK_FLAGS) 

Graphs.o: Graphs.c 
	$(CC)  $(LXRT_CFLAGS) -c -o $@ $< $(GTK_FLAGS) 

Logs.o: Logs.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(GTK_FLAGS) 

Neuron.o: Neuron.c 
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(GTK_FLAGS)      # LXRT_CFLAGS to see warnings during compile



clean:
	rm -f *.o NeuroSim

.PHONY: clean
