# Pipeline Processing Framework

This repository presents a conceptual Pipeline Processing Framework. For further information the reader can consult the following article by Danny Wang: 

A Generic Data Process Pipeline Library (https://edn.embarcadero.com/article/30360) 

As this article mentiones, the purpose of a pipeline of processor units is to get result data after the data flows through the pipeline. A pipeline can have an arbitrary number of process steps. In our definition, each process step has one process unit. Different pipeline steps do different jobs; hence they might be implemented as different classes. 


# General architecture

The basic system design architecture for this framework is adapted from GNU Radio Project (https://github.com/gnuradio/gnuradio).

# Different Object Types

Here are brief descriptions of various object data types. The remaining sections of this chapter go into more detail for objects of each type. 

 * Polymorphic Data Type (pmt): Polymorphic Data Type class plays an important role in our pipeline design architecture. It is been used for data exchange between different processor modules in the pipeline. A pmt data type is a (possibly infinite) set of C++ type objects. The data types defined in pmt are arranged into a hierarchy defined by the subset relationship. Among the variety of pmt types, pmt_genVector is used extensively for data exchange between processor modules. pmt pmt_genVector defines a generic vector. The pmt_genVector acts like a wrapper class around C++ std::vector class. A pmt object with pmt_genVector datatype can be created by using a series of global helper functions which their prototype declarations are defined in the pmt.h header file. 

 * Signal/Slot Design Pattern: It is being developed based an article by Simon Schneegans: What’s the Signal/Slot Pattern? (https://schneegans.github.io/tutorials/2015/09/20/signal-slot.html). Signal/Slot or Observer pattern is used for sending a pmt datatype from a processor module in the pipeline to another one. Basically, the Signal / Slot Pattern allows for event based inter-object communication. 

 * Object ID: An ObjectID class is implemented to create unique IDs for each run-time generated data packet in the pipeline. It’s mainly used for easy tracking, logging, and debugging purposes by tagging each packet since its existence. 

 * Processor: The Processor class is the interface pure abstract base class for a variety of Processor modules. It is being used by the Processor Factory class to create new processor nodes for the pipeline. It is being connected to the rest of the pipeline over different input and output ports. All the input and output ports are basically pmt datatype and are connected to the neighboring nodes in the pipeline over Signal/Slot observer design pattern.

 * Adder: This processing block adds samples across all input streams.

 * Vector Source: This processing block produces a stream of samples based on an input vector. 

 * Vector Sink: This processing block can be used to sink the input and write it to some other external utilities like graphic graph drawer (It is TBD).

 * Heterogeneous Container: Is is based on an article by Andy G: A true heterogeneous container in C++ (https://gieseanw.wordpress.com/2017/05/03/a-true-heterogeneous-container-in-c/).

 * Logger: It allows the running code to provide a trace of its execution in a series of log files. 


# JSON Configuration File

The run-time processing pipeline of the implemented framework is constructed based on a configuration file which is properly formatted and human-readable JSON file. A tiny and standalone JSON library for C++11 (https://github.com/dropbox/json11) is utilized to provide JSON parsing and serialization. 


# System Builder

The concept of system builder utilizes the notions for these articles: 

 * Choose template based run-time string in C++  (https://stackoverflow.com/questions/38644146/choose-template-based-on-run-time-string-in-c) 
 * Variadic templates in C++ by Eli Bendersky (https://eli.thegreenplace.net/2014/variadic-templates-in-c/) 

The System Builder class is aimed to construct and run the processing pipeline.



# Contributing

If you'd like to add or improve this software design, your contribution is welcome!


# License

This repository is released under the [MIT license](https://opensource.org/licenses/MIT). In short, this means you are free to use this software in any personal, open-source or commercial projects. Attribution is optional but appreciated.
