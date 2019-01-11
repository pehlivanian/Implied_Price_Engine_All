#ifndef __IMPLIEDSERVER_IMPL_HPP__
#define __IMPLIEDSERVER_IMPL_HPP__

#define QUOTE QuotePublishEvent
#define MAKE_QUOTE(A, B) QUOTE(std::make_pair((A), (B)))
#define SPREAD_QUOTE_PAIR std::pair< SecPair, QUOTE>
#define MAKE_SPREAD_QUOTE_PAIR(A, B, C) std::make_pair((A), MAKE_QUOTE((B), (C)))

static std::regex leg_pat(R"((Leg_)([\d]+))");
static std::regex spread_pat(R"((Spread_)([\d]+)[_]([\d]+))");

const int NUM_QUOTES = 1000000;

template<int N>
class ImpliedServer;

template<int N>
struct impl<ImpliedServer<N>>
{
    impl(bool sim_batch_mode, bool sim_realtime_mode, bool sync_mode, int port) :
            sim_batch_mode_(sim_batch_mode),
            sim_realtime_mode_(sim_realtime_mode),
            sync_mode_(sync_mode),
            IE_(std::make_unique<ImpliedEngine<N>>()),
            C_(std::make_unique<Client>(port, (char*)"0.0.0.0")),
            QS_(std::make_unique<QuoteSimulator<N>>(NuM_QUOTES))
           {}

    bool sim_batch_mode_;
    bool sim_realtime_mode_;
    bool sync_mode_;
    std::unique_ptr<ImpliedEngine<N>> IE_;
    std::unique_ptr<Client> C_;
    std::unique_ptr<QuoteSimulator<N>> QS_;
};

template<int N>
void
ImpliedServer<N>::init_()
{
    if (p_->sim_batch_mode_)
        (p_->C_)->fetch();
}

template<int N>
void
ImpliedServer<N>::process() {
    if (p_->sim_batch_mode_) {
        preload_tasks_();
        process_tasks_();
    } else if (p_->sim_realtime_mode_ ) {
        ;
    }
};

template<int N>
void
ImpliedServer<N>::process_profiled() {
    preload_tasks_();
    profiled_process_tasks_();
}

template<int N>
void
ImpliedServer<N>::profiled_process_tasks_()
{
    const int R = 5;
    const int C = tasks_.size();

#if 0
    std::string WORK_DIR = "~/ClionProjects/Implied_Price_Engine_All/data/";

    std::ofstream fsu("./data/user_quote.dat");
    std::ofstream fsi("./data/implied_quote.dat");
    auto q = (p_->IE_)->get_user_quote();
    fsu << "Type";
    fsi << "Type";
    for(size_t i=0; i<q[0].size(); ++i)
    {
        fsu << ",Leg_" + std::to_string(i);
        fsi << ",Leg_" + std::to_string(i);
    }
    fsu << "\n";
    fsi << "\n";
#endif

    long* Micro_times[R];

    for(size_t i=0; i<R; ++i)
    {
        Micro_times[i] = (long *) malloc(C * sizeof(long));
    }

    struct timeval beforeV, afterV;

    for(int r=0; r<R; ++r)
            for(int c=0; c<C; ++c)
            {
                gettimeofday(&beforeV, 0);
                tasks_[c]();
                gettimeofday(&afterV, 0);
                Micro_times[r][c] = diffTimer(&beforeV, &afterV);
#if 0
                if ((r == 0) && (c > 99) && ((c%10) == 0))
                {

                    (p_->IE_)->write_user_quote(c, fsu);
                    (p_->IE_)->write_implied_quote(c, fsi);

                }
#endif
            }

#if 0
    fsu.close();
    fsi.close();
#endif

    printf("Table (micros) for Implied Quote Update Step\n");
    printf ("n\taverage\t\tmin\tmax\tstdev\t\t#\n");
    buildTable(Micro_times, R, C);

}

template<int N>
void
ImpliedServer<N>::process_tasks_()
{
    for(auto& task : tasks_) {
        task();
        (p_->IE_)->write_user_curve();
        (p_->IE_)->write_implied_curve();
    }

}


SPREAD_QUOTE_PAIR
handler_(const rapidjson::Document &document, std::string typ) {

    using namespace std;
    using namespace rapidjson;

    string Inst = document["Inst"].GetString();
    smatch match;
    SecPair sp;
    int leg, spd0, spd1, pc;
    size_t sz;

    if (regex_match(Inst, match, leg_pat)) {
        istringstream ss(match[2]);
        ss >> leg;
        sp = SecPair(leg, -1, 1);
        pc = document[typ.c_str()].GetInt();
        sz = static_cast<size_t>(document["size"].GetInt());
    }
    else if (regex_match(Inst, match, spread_pat)) {
        istringstream ss0(match[2]), ss1(match[3]);
        ss0 >> spd0; ss1 >> spd1;
        sp = SecPair(spd0, spd1, 1);
        pc = document[typ.c_str()].GetInt();
        sz = static_cast<size_t>(document["size"].GetInt());
    }
    else {
        throw ImpliedServerException();
    }

    return MAKE_SPREAD_QUOTE_PAIR( sp, pc, sz);

}

template<int N>
void
ImpliedServer<N>::quote_handler_(const rapidjson::Document &document) {

    std::function<void()> publisher;

    if (document.HasMember("bid")) {
        auto q = handler_(document, std::string("bid"));
        // publisher = [this,q]() { return (p_->IE_)->publish_bid(q.first, q.second); };
        if (!(p_->sync_mode_)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            publisher = [this, q]() {
                // (this->p_->IE_)->write_user_curve();
                // (this->p_->IE_)->write_implied_curve();
                return (p_->IE_)->publish_bid(q.first, q.second);
            };
        } else {
            publisher = [this, q]() { return (p_->IE_)->publish_bid(q.first, q.second); };
        }
    }
    else if (document.HasMember("ask")) {
        auto q = handler_(document, std::string("ask"));
        // publisher = [this,q]() { return (p_->IE_)->publish_ask(q.first, q.second); };
        if (!(p_->sync_mode_)) {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
            publisher = [this, q]() {
                // (this->p_->IE_)->write_user_curve();
                // (this->p_->IE_)->write_implied_curve();
                return (p_->IE_)->publish_ask(q.first, q.second);
            };
        } else {
            publisher = [this,q]() { return (p_->IE_)->publish_ask(q.first, q.second); };
        }
    }

    if (p_->sync_mode_) {
        tasks_.push_back(publisher);
    } else {
        pool_.submit(publisher);
    }

}

template<int N>
void
ImpliedServer<N>::preload_tasks_()
{
    rapidjson::Document document;

    std::string tok;

    std::stringstream ss((p_->C_)->get_buf());

    while(getline(ss, tok, '\n'))
    {
        if (document.Parse(tok.c_str()).HasParseError())
        {
            fprintf(stderr, "Parse error!\n");
            return;
        };

        assert(document.HasMember("Inst"));

        quote_handler_(document);

    }

#undef MAKE_SPREAD_QUOTE_PAIR
#undef SPREAD_QUOTE_PAIR
#undef MAKE_QUOTE
#undef QUOTE
}

#endif
