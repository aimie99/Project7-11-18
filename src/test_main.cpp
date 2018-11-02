#include <gtest/gtest.h>
#include <algorithm>
#include "network.h"
#include "random.h"

#define MAX 15 

RandomNumbers RNG(101);
Network net;

TEST(networkTest, initialize) {
    net.resize(MAX);
    EXPECT_EQ(net.size(), MAX);
    std::vector<double> vals = net.sorted_values();
    EXPECT_TRUE(std::is_sorted(vals.begin(), vals.end(), std::greater<double>()));
    double var = 0;
    for (auto I : vals) var += I*I;
    EXPECT_GT(var, sqrt(MAX));
}

TEST(networkTest, connect) {
    bool trylink = net.add_link(0,0);
    EXPECT_FALSE(trylink);
    trylink = net.add_link(0, MAX);
    EXPECT_FALSE(trylink);
    trylink = net.add_link(MAX-2, MAX-1);
    EXPECT_TRUE(trylink);
    
    trylink = net.add_link(MAX-2, MAX-1);
    EXPECT_FALSE(trylink);
    
    EXPECT_EQ(net.degree(MAX-2)+net.degree(MAX-1), 2);    
    trylink = net.add_link(MAX-2,MAX-3);
    trylink = net.add_link(1,MAX-2);
    std::vector<size_t> ngb = net.neighbors(MAX-2);
    EXPECT_EQ(ngb.size(), 3);
    /*EXPECT_TRUE(ngb[0] == MAX-1 && ngb[1] == MAX-2 && ngb[2] == 1);*/
    
    double numlink = 0;
    for (int rep(0); rep < 100; rep++) 
      numlink += 0.01 * net.random_connect(2);
    EXPECT_NEAR(numlink, 2*MAX, 15);
}

TEST(networkTest, values) {
    std::vector<double> vals{1.0, 10.0, -0.4, .2, -21.8, 11.0};
    size_t numv = net.set_values(vals);
    EXPECT_EQ(numv, 6);
    double checksum = 0;
    for (size_t n=0; n<6; n++) checksum += net.value(n);
    EXPECT_NEAR(checksum, 0.0, 0.01);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
