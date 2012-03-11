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

BMISimulationSpikeGenerator: BMISimulationSpikeGenerator.c ../../../BlueSpike/Library/Misc/Misc.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialTypes.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/ExpEnviCommandTimeStamp.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/ExpEnviStatus.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/ExpEnviEventTimeStamp.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialControllerGuiRequestTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/MovObjCommandTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/MovObjEventTimeStamp.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o
	$(CC) $(LXRT_CFLAGS) $(LXRT_LDFLAGS) $(MATH_FLAGS) $(GTK_FLAGS) -o $@ $<  ../../../BlueSpike/Library/Misc/Misc.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialsData.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialTypes.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialStatusTypes.o  ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/ExpEnviCommandTimeStamp.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/ExpEnviStatus.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/ExpEnviEventTimeStamp.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/TrialControllerGuiRequestTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/MovObjCommandTimeStamp.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/MovObjEventTimeStamp.o ../../../BlueSpike/Library/RtTaskStats/RtTaskStats.o ### BMISimulationSpikeGenerator

../../../BlueSpike/Library/Misc/Misc.o: ../../../BlueSpike/Library/Misc/Misc.c
	$(CC) $(LXRT_CFLAGS) -c -o $@ $< $(LXRT_LDFLAGS) $(GTK_FLAGS)			### ../../../BlueSpike/Library/Misc/Misc

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

clean:
	rm -f BMISimulationSpikeGenerator *.o  ../../../BlueSpike/Library/Misc/*.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/*.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviCommands/*.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviStatus/*.o ../../../ExperimentControllers/ExpEnviHandlers/Library/ExpEnviEvents/*.o ../../../ExperimentControllers/TrialControllers/Library/TrialsData/*.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjCommands/*.o ../../../ExperimentControllers/MovObjHandlers/Library/MovObjEvents/*.o ../../../BlueSpike/Library/RtTaskStats/*.o

.PHONY: clean

