#pragma once
#ifndef MEAT_WAGONS_APPLICATION_H
#define MEAT_WAGONS_APPLICATION_H

using namespace std;

class Application {
    enum operationType {READ_GRAPH, PRE_PROCESS, SHORTEST_PATH_1, SHORTEST_PATH_2, SHORTEST_PATH_3, SHORTEST_PATH_4
        TSP_PROBLEM, DISPATCH_POSITION, DD_REQUEST, REMOVE_REQUEST, LIST_REQUESTS, ADD_WAGON, REMOVE_WAGON,
        LIST_WAGONS, GRAPH_CONNECTIVITY, INTERESTS_POINTS_CONNECTIVITY};

    private:
        operationType operation;
        vector<string> operands;

    public:
        Application(int argc, char* argv[]) {
            if(argc < 2) throw "Incorrect number of parameters";

            switch (argv[1]) {
                case "readGraph":
                    if(argc < 3) throw "Incorrect number of parameters";
                    this->operation = READ_GRAPH;
                    this->operands.push_back(argv[2]);
                    break;

                case "preProcess":
                    if(argc < 3) throw "Incorrect number of parameters";
                    this->operation = PRE_PROCESS;
                    this->operands.push_back(argv[2]);
                    break;

                case "shortestPath":
                    switch(argv[2]) {
                        case "dijkstra":
                            if(argc < 4) throw "Incorrect number of parameters";
                            this->operation = SHORTEST_PATH_1
                            this->operands.push_back(argv[3]);
                            break;

                        case "dijkstra":
                            if(argc < 5) throw "Incorrect number of parameters";
                            this->operation = SHORTEST_PATH_1
                            this->operands.push_back(argv[3]);
                            this->operands.push_back(argv[4]);
                            break;

                        case "dijkstraOriented":
                            if(argc < 5) throw "Incorrect number of parameters";
                            this->operation = SHORTEST_PATH_2
                            this->operands.push_back(argv[3]);
                            this->operands.push_back(argv[4]);
                            break;

                        case "dijkstraBidirectional":
                            if(argc < 5) throw "Incorrect number of parameters";
                            this->operation = SHORTEST_PATH_3
                            this->operands.push_back(argv[3]);
                            this->operands.push_back(argv[4]);
                            break;

                        case "dijkstraOrientedBidirectional":
                            if(argc < 5) throw "Incorrect number of parameters";
                            this->operation = SHORTEST_PATH_4;
                            this->operands.push_back(argv[3]);
                            this->operands.push_back(argv[4]);
                            break;

                        default:
                            break;
                    }

                case default:
                    usage();
                    break;
            }
        }

        public static void usage() const;
        public void run() const;
};

public Application::usage() const {
    cout << "Usage:" << endl;
    cout << "./meat-wagons readGraph <folder path>";
    cout << "./meat-wagons preProcess <node id>";
    cout << "./meat-wagons shortestPath dijkstra <origin node>";
    cout << "./meat-wagons shortestPath dijkstra <origin node> <destination node>";
    cout << "./meat-wagons shortestPath dijkstraOriented <origin node> <destination node>";
    cout << "./meat-wagons shortestPath dijkstraBidirectional <origin node> <destination node>";
    cout << "./meat-wagons shortestPath dijkstraOrientedBidirectional <origin node> <destination node>";
}

public Application::run() const {
    switch(this->operation) {
        case READ_GRAPH:
            break;
        case PRE_PROCESS:
            break;
        case SHORTEST_PATH_1:
            break;
        case SHORTEST_PATH_2:
            break;
        case SHORTEST_PATH_3:
            break;
        case SHORTEST_PATH_4:
            break;
        default:
            break;
    }
}

#endif //MEAT_WAGONS_APPLICATION_H