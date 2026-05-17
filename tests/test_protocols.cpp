#include <gtest/gtest.h>
#include "IP.h"
#include "CSMA_CD.h"
#include "Packet.h"

TEST(ProtocolTest, EncapsulationStrategies) {
    Packet testPkt(555, "1.1.1.1", "2.2.2.2", 10, 1500);

    // Case 1: Инкапсуляция IP (Сетевой уровень)
    IP ipProtocol;
    Frame ipFrame = ipProtocol.encapsulate(testPkt);
    EXPECT_EQ(ipFrame.protocolHeader, "IPv4_Header[Proto:6,DSCP:0]");
    EXPECT_EQ(ipFrame.payload.getId(), 555);

    // Case 2: Инкапсуляция CSMA/CD (Канальный уровень)
    CSMA_CD ethProtocol;
    Frame ethFrame = ethProtocol.encapsulate(testPkt);
    EXPECT_EQ(ethFrame.protocolHeader, "Ethernet_II_Header[Type:0x0800,Preamble:CSMA/CD]");
    EXPECT_EQ(ethFrame.payload.getId(), 555);

    // Case 3: Сохранность данных пакета после инкапсуляции
    EXPECT_EQ(ethFrame.payload.getSrcIP(), "1.1.1.1");
    EXPECT_EQ(ethFrame.payload.getTTL(), 10);
    EXPECT_EQ(ethFrame.payload.getSize(), 1500);
}