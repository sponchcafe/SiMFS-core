#pragma once

#include <string>
#include <memory>

namespace sim{
    namespace graph{

        std::string const EMPTY_NAME = "";
        std::string const DEFAULT_NODE_NAME = "__default_node__";
        std::string const DEFAULT_EDGE_NAME = "__default_edge__";
        std::string const DEFAULT_ACTION_NAME = "__default_action__";

        class Node;
        class Edge;
        class Graph;
        class Event;
        class Action;

    }
}
