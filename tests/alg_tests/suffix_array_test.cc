#include "gtest/gtest.h"
#include "suffix_array.h"
#include "pretty_print.h"

namespace alg {
namespace {

TEST(SuffixArray,SimpleCompress){

  std::vector<unsigned int> v = {2,3,2,4,23,13,12,5,13,13,12,12,11,5,4};
  EXPECT_EQ(alg::single_compress(v.begin(),v.end(),v.begin()),8);
  EXPECT_EQ(v,std::vector<unsigned int>({0, 1, 0, 2, 7, 6, 5, 3, 6, 6, 5, 5, 4, 3, 2}));
}

TEST(SuffixArray,RadixCompressUnsigned){
  std::vector<unsigned int> v = {2,3,2,4,23,13,12,5,13,13,12,12,11,5,4};
  EXPECT_EQ(alg::radix_compress(v.begin(),v.end(),v.begin()),8);
  EXPECT_EQ(v,std::vector<unsigned int>({0, 1, 0, 2, 7, 6, 5, 3, 6, 6, 5, 5, 4, 3, 2}));
}

TEST(SuffixArray,RadixCompressSigned){
  std::vector<int> v = {2,3,2,4,23,13,12,-5,-13,-13,12,12,11,5,4};
  std::vector<uint16_t> vc(v.size());
  EXPECT_EQ(alg::radix_compress(v.begin(),v.end(),vc.begin()),10);
  EXPECT_EQ(vc,std::vector<uint16_t >({2, 3, 2, 4, 9, 8, 7, 1, 0, 0, 7, 7, 6, 5, 4}));
}

TEST(SuffixArray,SuffixBlockCompress){
  std::vector<uint16_t> v = {2,1,3,1,3,1};
  EXPECT_EQ(alg::block_radix_compress<2>(v.begin(),v.end(),v.begin()),4);
  EXPECT_EQ(v,std::vector<uint16_t >({2,1,3,1,3,0}));
}

TEST(SuffixArray,SuffixCompress){
  std::vector<uint16_t> v = {2,1,3,1,3,1};
  alg::compress_suffix_nlogn(v.begin(),v.end(),v.begin());
  //EXPECT_EQ(v,std::vector<uint16_t >({5,2,4,1,3,0}));
}

TEST(SuffixArray,Simple){

}

}
}