#include "ImpliedEngine_test.hpp"

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
int main(int argc, char **argv)
{

  // Client* C = new Client(8080, "127.0.0.1");
  // char* file_buf = C->get();
  // std::cout << std::string(file_buf) << "\n\n";

  // quote_msg *qm = new quote_msg(file_buf);

    using IE8 = ImpliedEngine<8>;
    IE8* IE = new IE8;

    // Test publishing mechanism
    IE->publish_bid(SecPair(0, -1, 1), QUOTE(4504, 1));
    IE->publish_bid(SecPair(1, -1, 1), QUOTE(4604, 1));
    IE->publish_bid(SecPair(2, -1, 1), QUOTE(4704, 1));
    IE->publish_bid(SecPair(3, -1, 1), QUOTE(4804, 1));
    IE->publish_bid(SecPair(4, -1, 1), QUOTE(4904, 1));
    IE->publish_bid(SecPair(5, -1, 1), QUOTE(5004, 1));
    IE->publish_bid(SecPair(6, -1, 1), QUOTE(5104, 1));
    IE->publish_bid(SecPair(7, -1, 1), QUOTE(5204, 1));
    
    IE->publish_ask(SecPair(0, -1, 1), QUOTE(4514, 1));
    IE->publish_ask(SecPair(1, -1, 1), QUOTE(4614, 1));
    IE->publish_ask(SecPair(2, -1, 1), QUOTE(4714, 1));
    IE->publish_ask(SecPair(3, -1, 1), QUOTE(4814, 1));
    IE->publish_ask(SecPair(4, -1, 1), QUOTE(4914, 1));
    IE->publish_ask(SecPair(5, -1, 1), QUOTE(5014, 1));
    IE->publish_ask(SecPair(6, -1, 1), QUOTE(5114, 1));
    IE->publish_ask(SecPair(7, -1, 1), QUOTE(5214, 1));


    IE->publish_ask(SecPair(1, -1, 1), QUOTE(4610, 14));
    IE->publish_ask(SecPair(0,  3, 1), QUOTE(-300, 11));
    IE->publish_bid(SecPair(1,  3, 1), QUOTE(-200, 12));

    IE->publish_bid(SecPair(0,2,1), QUOTE(-200,  247));
    IE->publish_bid(SecPair(2,3,1), QUOTE(-100,  55));
    IE->publish_bid(SecPair(3,-1,1), QUOTE(4805, 88));

    IE->write_user_curve();
    IE->write_implied_curve();
    IE->write_merged_curve();

    auto Graphs = IE->get_Graphs();

    IE->write_dot(0, (char*)"leg_0.dot", Graphs[0]->get_distance(0), Graphs[0]->get_predecessor(0));
  
}
