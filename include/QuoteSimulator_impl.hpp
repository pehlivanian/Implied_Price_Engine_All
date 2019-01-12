#ifndef __QUOTESIMULATOR_IMPL_HPP__
#define __QUOTESIMULATOR_IMPL_HPP__

int move_bid_price(int bid, int ask, int move_type, int inc, bool is_spread=false)
{
    int r;
    int spread_inc = 1;
    if (is_spread)
        spread_inc = 1;

    switch(move_type)
    {
        case (0) :
            r = std::min(bid+inc, ask-spread_inc);
            break;
        case (1) :
            r = std::min(bid+inc, ask-spread_inc);
            break;
        case (2) :
            r = std::min(bid+inc, ask-spread_inc);
            // r = std::min(static_cast<int>(.5*(bid + ask)), ask-1);
            break;
    }
    return r;
}

int move_ask_price(int bid, int ask, int move_type, int inc, bool is_spread=false)
{
    int r;
    int spread_inc = 1;
    if (is_spread)
        spread_inc = 1;

    switch(move_type)
    {
        case (0) :
            r = std::max(ask-inc,bid+spread_inc);
            break;
        case (1) :
            r = std::max(ask-inc,bid+spread_inc);
            break;
        case (2) :
            r = std::max(ask-inc,bid+spread_inc);
            // r = std::max(static_cast<int>(.5*(bid + ask + 2)), bid+1);
            break;
    }
    return r;
}


template<int N>
void
QuoteSimulator<N>::create_quotes_()
{
    int quote_count = 0;
    int num_legs = UE_->get_num_legs();

    for(size_t i=0; i<num_legs; ++i) {
        quote q = quote(quote::QUOTE_TYPE::Bid, SecPair(i, -1, 1), 4604 + (i * 2), 10);
        quotes_[quote_count++] = q;
        UE_->publish_bid(q.sp_, std::make_pair(q.p_, q.s_));
    }
    for(size_t i=0; i<num_legs; ++i) {
        quote q = quote(quote::QUOTE_TYPE::Ask, SecPair(i, -1, 1), 4606 + (i * 4), 10);
        quotes_[quote_count++] = q;
        UE_->publish_ask(q.sp_, std::make_pair(q.p_, q.s_));
    }

    for(size_t i=0; i<num_legs; ++i)
        for(size_t j=i+1; j<num_legs; ++j)
        {
            int bid_price = UE_->get_user_bid(i).first - UE_->get_user_ask(j).first;
            int ask_price = UE_->get_user_ask(i).first - UE_->get_user_bid(j).first;
            int mid = static_cast<int>(.5 * (bid_price + ask_price));
            quote q1 = quote(quote::QUOTE_TYPE::Bid, SecPair(i,j,1), mid - i - (j-i-1), 11);
            quote q2 = quote(quote::QUOTE_TYPE::Ask, SecPair(i,j,1), mid,               11);
            quotes_[quote_count++] = q1;
            quotes_[quote_count++] = q2;
            UE_->publish_bid(q1.sp_, std::make_pair(q1.p_, q1.s_));
            UE_->publish_ask(q2.sp_, std::make_pair(q2.p_, q2.s_));
        }

    std::mt19937 gen;
    gen.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist_leg(0,num_legs-1);
    std::uniform_int_distribution<std::mt19937::result_type> dist_bid_ask(0,1);
    std::uniform_int_distribution<std::mt19937::result_type> dist_inst_type(0,2);
    std::uniform_int_distribution<std::mt19937::result_type> dist_inc(-1,1);
    std::uniform_int_distribution<std::mt19937::result_type> dist_size(1,25);
    std::uniform_int_distribution<std::mt19937::result_type> dist_price_move_type(0,2);

#define QUOTE(A, B) QuotePublishEvent(std::make_pair((A), (B)))
    while(quote_count < num_quotes_)
    {
        int leg_num = dist_leg(gen);
        int bid_ask = dist_bid_ask(gen);
        int inst_type = dist_inst_type(gen);
        int price_move_type = dist_price_move_type(gen);
        size_t sz = static_cast<size_t>(dist_size(gen));
        int inc = dist_inc(gen);

        int curr_price;
        int curr_bid0 = UE_->get_user_bid(leg_num).first;
        int curr_ask0 = UE_->get_user_ask(leg_num).first;
        bool mkts_locked = false;
        quote::QUOTE_TYPE t = (quote::QUOTE_TYPE)(bid_ask);
        if ((inst_type == 0) || (leg_num == num_legs-1)) {
            switch (t) {
                case quote::QUOTE_TYPE::Bid :
                    curr_price = move_bid_price(curr_bid0, curr_ask0, price_move_type, inc);
                    try {
                        UE_->publish_bid(SecPair(leg_num, -1, 1), std::make_pair(curr_price, sz));
                    }
                    catch(...)
                    {
                        // Walk it back
                        mkts_locked = true;
                    }
                    break;
                case quote::QUOTE_TYPE::Ask :
                    curr_price = move_ask_price(curr_bid0, curr_ask0, price_move_type, inc);
                    try {
                        UE_->publish_ask(SecPair(leg_num, -1, 1), std::make_pair(curr_price, sz));
                    }
                    catch(...)
                    {
                        // Walk it back
                        mkts_locked = true;
                    }
                    break;
            }
//            std::cerr << "DEBUG: " << SecPair(leg_num, -1, 1) << " : "
//                      << (quote::QUOTE_TYPE(bid_ask)) << " : "
//                      << curr_bid0 << " : " << curr_ask0 << " : "
//                      << move_bid_price << " ::: " << curr_price << "\n";
            if (!mkts_locked)
                quotes_[quote_count++] = quote(t, SecPair(leg_num, -1, 1), curr_price, sz);
        }
        else
        {
            std::uniform_int_distribution<std::mt19937::result_type> dist_leg1(leg_num+1,num_legs-1);
            int leg1_num = dist_leg1(gen);
            int curr_bid1 = UE_->get_user_bid(leg1_num).first;
            int curr_ask1 = UE_->get_user_ask(leg1_num).first;
            int mid = .5 * ((curr_bid0 - curr_ask1) + (curr_ask0-curr_bid1));

            switch (t) {
                case quote::QUOTE_TYPE::Bid :
                    curr_price = move_bid_price(mid - leg_num - (leg1_num-leg_num-1), mid, price_move_type, inc);
                    try {
                        UE_->publish_bid(SecPair(leg_num, leg1_num, 1), std::make_pair(curr_price, sz));
                    }
                    catch(...)
                    {
                        // Walk it back
                        mkts_locked = true;
                    }
                    break;
                case quote::QUOTE_TYPE::Ask :
                    curr_price = move_ask_price(mid,  mid + leg_num + (leg1_num-leg_num-1), price_move_type, inc);
                    try {
                        UE_->publish_ask(SecPair(leg_num, leg1_num, 1), std::make_pair(curr_price, sz));
                    }
                    catch(...)
                    {
                        // Walk it back
                        mkts_locked = true;
                    }
                    break;
            }
//            std::cout << "DEBUG: " << SecPair(leg_num, leg1_num, 1) << " : "
//                      << (quote::QUOTE_TYPE(bid_ask)) << " : "
//                      << curr_bid0-curr_ask1 << " : " << curr_ask0-curr_bid1 << " : "
//                      << curr_price << "\n";
            if (!mkts_locked)
                quotes_[quote_count++] = quote(t, SecPair(leg_num, leg1_num, 1), curr_price, sz);
        }
    }
#undef QUOTE

}


template<int N>
void
QuoteSimulator<N>::attach( std::function<void(const rapidjson::Document &)> callback) {
    rapidjson::Document document;
    std::ostringstream stream;


    for (auto q: quotes_) {
        stream << q;
        std::string streamString = stream.str();
        if (document.Parse(streamString.c_str()).HasParseError()) {
            fprintf(stderr, "Parse error!");
            return;
        }
        callback(std::forward<rapidjson::Document>(document));
    }
}

template<int N>
void
QuoteSimulator<N>::dump() {
    std::for_each(quotes_.begin(), quotes_.end(), [](auto &q){ std::cout << q; });
}

template<int N>
void
QuoteSimulator<N>::dump_to_file(std::ofstream& ofs) {
  std::for_each(quotes_.begin(), quotes_.end(), [&ofs](auto &q){ ofs << q; });
}

#if 0
// Refactor this with rapidjson eventually
template<int N>
void
QuoteSimulator<N>::serialize_json() const
{
    std::fstream fs;
    fs.open(filename_, "w");

    for(auto& quote : quotes_)
        fs << quote;
}
#endif

#endif
