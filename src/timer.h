class Timer {
private:
  unsigned long resetTime;
  unsigned long limit;
public:
  Timer(unsigned long limit);
  void reset();
  bool elapsed();
};
