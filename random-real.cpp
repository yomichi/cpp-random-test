#include <iostream>
#include <boost/timer/timer.hpp>
#include <random>
#include <boost/random.hpp>
#include <boost/lexical_cast.hpp>
#include <gsl/gsl_rng.h>
#include "dSFMT-src-2.2.1/dSFMT.h"

int main(int argc, char **argv)
{
  std::random_device rng_dev;

  long long N = argc>1 ? boost::lexical_cast<long long>(argv[1]) : 10000*10000;
  double sum=0.0;

  boost::timer::cpu_timer timer;
  double ref_time = 0.0;

  {
    timer.start();
    for(long long i=0; i<N; ++i)
      sum += i;
    timer.stop();
  }
  if(sum>0.0)
    ref_time = timer.elapsed().wall*1.0e-9;

  sum = 0.0;
  std::cout << "C++11 : ";
  const size_t seed_size = 1;
  auto std_seed = [seed_size, &rng_dev](){
    std::vector<std::random_device::result_type> i(seed_size);
    std::generate(i.begin(), i.end(), std::ref(rng_dev));
    return std::seed_seq(i.begin(), i.end());
  }();
  std::mt19937 std_mt(std_seed);
  std::uniform_real_distribution<double> std_dist(0.0,1.0);
  {
    timer.start();
    for(long long i=0; i<N; ++i)
      sum +=  std_dist(std_mt);
    timer.stop();
  }
  if(sum>0.0)
    std::cout << timer.elapsed().wall*1.0e-9 - ref_time << std::endl;

  sum = 0.0;
  std::cout << "Boost : ";
  boost::variate_generator<boost::mt19937, boost::uniform_real<>>
    boost_rng(boost::mt19937(rng_dev()), boost::uniform_real<>(0.0,1.0));
  {
    timer.start();
    for(long long i=0; i<N; ++i)
      sum +=  boost_rng();
    timer.stop();
  }
  if(sum>0.0)
    std::cout << timer.elapsed().wall*1.0e-9 - ref_time << std::endl;

  sum = 0.0;
  std::cout << "dSFMT : ";
  dsfmt_t dsfmt_rng;
  dsfmt_init_gen_rand(&dsfmt_rng, rng_dev());
  {
    timer.start();
    for(long long i=0; i<N; ++i)
      sum +=  dsfmt_genrand_close_open(&dsfmt_rng);
    timer.stop();
  }
  if(sum>0.0)
    std::cout << timer.elapsed().wall*1.0e-9 - ref_time << std::endl;

  sum = 0.0;
  std::cout << "GSL   : ";
  gsl_rng * gsl_rng = gsl_rng_alloc(gsl_rng_mt19937);
  gsl_rng_set(gsl_rng, rng_dev());
  {
    timer.start();
    for(long long i=0; i<N; ++i)
      sum +=  gsl_rng_uniform(gsl_rng);
    timer.stop();
  }
  if(sum>0.0)
    std::cout << timer.elapsed().wall*1.0e-9 - ref_time << std::endl;
}
