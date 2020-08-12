/**
 * @file   processor.h
 *
 * @author Armin Zare Zadeh ali.a.zarezadeh@gmail.com
 *         Eric J. Mayo eric@pozicom.net
 *
 * @brief  processor.h includes the interface for all processing classes
 */

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "tags.h"
#include "signal_slot.h"

#include <cstdint>
#include <memory>
#include <complex>
#include <vector>
#include <list>
#include <tuple>
#include <mutex>

namespace pl_proc {

/*!
 * \brief processor abstract class interface as base parent class for all processor nodes.
 */
class processor
{
protected:
  /*!
   * \brief Number of output items on processor node
   */
  uint32_t noutput_items_;

  /*!
   * \brief Polymorphic type Output items vector from processor node
   */
  pmt::pmt_t output_items_;

  /*!
   * \brief Polymorphic type First Input items vector to processor node
   */
  pmt::pmt_t input_items1_;

  /*!
   * \brief Mutex 
   */
  std::mutex mutex_;

private:
  /*!
   * \brief Processor Module Type
   */
  ObjectIDModuleType moduleType_;

  /*!
   * \brief Processor Module Index
   */
  ObjectIDModuleIndexType moduleIndex_;

  /*!
   * \brief Processor Module Name
   */
  std::string moduleName_;

  /*!
   * \brief Processor Adjacency Connection List
   */
  std::list<std::tuple<std::string, std::string, std::string>> adjacencyConnection_;

  /*!
   * \brief Trigger Start: Indicates this processor node must be triggered to start at run simulation
   */
  bool trigStart_;

  /*!
   * \brief Packet Index
   */
  ObjectIDPaketIndexType paketIndex_;

  /*!
   * \brief Signals and slots Observer Pattern which notifies the generation of a new TAG
   */
  std::shared_ptr<signal_slot<tag_t&>> onNewTag_;

  /*!
   * \brief Signals and slots Observer Pattern which notifies the generation of a new OUTPUT DATA
   */
  std::shared_ptr<signal_slot<pmt::pmt_t&>> onNewData_;

  /*!
   * \brief Signals and slots Observer Pattern which notifies the reception of a new FIRST INPUT DATA
   */
  std::shared_ptr<signal_slot<>> onFirstInputSet_;

public:
  processor(ObjectIDModuleType moduleType,
            ObjectIDModuleIndexType moduleIndex,
            const std::string& moduleName,
            const std::list<std::tuple<std::string, std::string, std::string>>& adjacencyConnection,    
            uint32_t noutput_items,
            bool trigStart)
    : moduleType_(moduleType),
      moduleIndex_(moduleIndex),
      moduleName_(moduleName),
      adjacencyConnection_(adjacencyConnection),
      trigStart_(trigStart),
      noutput_items_(noutput_items),
      paketIndex_(0)
  {
    std::lock_guard<std::mutex> locker(mutex_);
    onNewTag_ = std::make_shared<signal_slot<tag_t&>>();
    onNewData_ = std::make_shared<signal_slot<pmt::pmt_t&>>();
    onFirstInputSet_ = std::make_shared<signal_slot<>>();     
  }

  virtual ~processor() {
    onNewTag_ = nullptr;
    onNewData_ = nullptr;
    onFirstInputSet_ = nullptr;
  };

  /*!
   * \brief Signals and slots Observer Pattern which emits a new TAG which wraps up the 
   *        generated new output data with timetag and packet index
   */
  void emitNewTag(pmt::DataType pmtValDataType)
  {
    int64_t timetag = current_time_ms();
    JobRunID *jobRunId = jobRunId->getInstance();
    const ObjectID objId = ObjectID::ForModuleIndex(*jobRunId,
                                                    moduleType_,
                                                    moduleIndex_,
                                                    paketIndex_);
    tag_t tag = tag_t(timetag, objId, pmtValDataType, output_items_);
    paketIndex_++;
    onNewTag_->emit(tag);
  }

  /*!
   * \brief Signals and slots Observer Pattern which emits a new output data
   */
  void emitNewData()
  {
    onNewData_->emit(output_items_);
  }

  /*!
   * \brief Signals and slots Observer Pattern which emits on reception of the first input data
   */
  void emitFirstInput()
  {
    onFirstInputSet_->emit();
  }

  /*!
   * \brief New type definition for a smart shared pointer of processor node
   */
  typedef std::shared_ptr<processor> sptr;

  /*!
   * \brief Getter interface for Processor Module Index
   */
  virtual ObjectIDModuleIndexType getModuleIndex() const { return moduleIndex_; };

  /*!
   * \brief Getter interface for Processor Module Name
   */
  virtual const std::string getModuleName() const { return moduleName_; };

  /*!
   * \brief Getter interface for Processor Module Adjacency Connection
   */
  virtual const std::list<std::tuple<std::string, std::string, std::string>> getAdjacencyConnection() const { return adjacencyConnection_; };

  /*!
   * \brief Main Process Method for Processor Module (Receives Input and Generates Output)
   */
  virtual void process(pmt::pmt_t& input_items) = 0;

  /*!
   * \brief Start Method for Processor Module (Generates Output)
   */
  virtual void start() = 0;

  /*!
   * \brief Getter interface for Processor Module to indicate it's done
   */
  virtual bool getDone() = 0;

  /*!
   * \brief Getter interface for Processor Module to indicate it's done
   */
  virtual void setInput1(pmt::pmt_t& input_items1) = 0;

  /*!
   * \brief Getter interface for the new TAG Signal/Slot of Processor Module Node
   */
  virtual std::shared_ptr<signal_slot<tag_t&>> getOnNewTag() { return onNewTag_; };

  /*!
   * \brief Getter interface for the new DATA Signal/Slot of Processor Module Node
   */
  virtual std::shared_ptr<signal_slot<pmt::pmt_t&>> getOnNewDataGen() { return onNewData_; };

  /*!
   * \brief Getter interface for the new DATA Signal/Slot of Processor Module Node
   */
  virtual std::shared_ptr<signal_slot<>> getFirstInputSet() { return onFirstInputSet_; };

  /*!
   * \brief Getter interface for Trigger Start Property of Processor Module Node
   */
  virtual bool getTrigStart() { return trigStart_; };
};

/*!
 * \brief Overloading the << operator for printing the Processor Module Node
 */
inline std::ostream& operator<<(std::ostream& os, const processor::sptr& dt)
{
  os << "fec::processor ModuleName:" << dt->getModuleName().c_str() << " ModuleIndex:" << unsigned(dt->getModuleIndex()) << " AdjacencyConnection:";
  for (auto const& i : dt->getAdjacencyConnection()) {
    os << "(" << std::get<0>(i) << ":" << std::get<1>(i) << ":" << std::get<2>(i) << ")";
  }
  return os;
}

} // namespace pl_proc

#endif /* PROCESSOR_H */
