cmake_minimum_required(VERSION 2.6)

include_directories(${CMAKE_SOURCE_DIR}/utils)

add_library(IElib STATIC    Trace
                            timing
                            report
                            benchmark
                            threadpool
                            LW_threadpool
                            Client
                            Visitor
                            DataElement
                            BinaryHeap
                            Graph
                            MarketGraph
                            SecPair
                            Decomposer
                            ImpliedQuoteSubscriber
                            CMemPool
                            cat1_visitor
                            cat2_visitor
                            ImpliedEngine
                            ImpliedServer
                            QuoteServer)
target_include_directories(IElib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})