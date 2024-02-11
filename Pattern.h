#ifndef PATTERN_H
#define PATTERN_H

class Pattern {
public:
  virtual void initialize() = 0;
  virtual void update() = 0;
  virtual ~Pattern() {}
};

#endif
