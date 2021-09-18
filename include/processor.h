#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp
  Processor(Processor *oldProcessor, int user, int nice, int system, int idle,
            int iowait, int irq, int softirq, int steal, int guest,
            int guest_nice);
  Processor();

  // TODO: Declare any necessary private members
 private:
  Processor *oldProcessor;
  int user;
  int nice;
  int system;
  int idle;
  int iowait;
  int irq;
  int softirq;
  int steal;
  int guest;
  int guest_nice;
};

#endif
