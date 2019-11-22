#ifndef TEMBLOR_DATAREADERS_MINISEED_TRACEGROUP_HPP
#define TEMBLOR_DATAREADERS_MINISEED_TRACEGROUP_HPP 1
#include <vector>
#include <memory>
#include "temblor/dataReaders/miniseed/enums.hpp"

namespace Temblor::Utilities
{
class Time;
}

namespace Temblor::DataReaders::MiniSEED
{
class SNCL;
class Trace;
class TraceGroup
{
public:
    /*! @name Constructors
     * @{
     */
    /*!
     * @brief Default constructor.
     */
    TraceGroup();
    /*!
     * @brief Copy constructor.
     * @param[in] traceGroup  The trace group class from which to initialize
     *                        this class.
     */
    TraceGroup(const TraceGroup &traceGroup);
    /*!
     * @brief Move constructor.
     * @param[in,out] traceGroup  The trace group class from which to
     *                            initialize this class.  On exit, traceGroup's
     *                            behavior is undefined.
     */
    TraceGroup(TraceGroup &&traceGroup) noexcept;
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] traceGroup  The trace group class to copy.
     * @result A deep copy of the trace group class.
     */
    TraceGroup& operator=(const TraceGroup &traceGroup);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] traceGroup  The trace groupclass whose memory will be
     *                            moved to this.  On exit, traceGroup's
     *                            behavior will be undefined.
     * @result The memory from traceGroup moved to this.
     */ 
    TraceGroup& operator=(TraceGroup &&traceGroup) noexcept;
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~TraceGroup();
    /*!
     * @brief Clears the class and releases memory.
     */
    void clear() noexcept;
    /*! @} */ 

    /*!
     * @brief Reads the miniSEED file.
     * @param[in] fileName  The name of the miniSEED file.
     * @throws std::invalid_argument if the miniSEED file does not exist or
     *         the file is malformed.
     */
    void read(const std::string &fileName);
    /*!
     * @brief Gets the SNCLs that exist in the archive.
     * @result The SNCLs that exist in the archive.  The result can be 
     *         empty if no data was read.
     * @sa \c read()
     */ 
    std::vector<SNCL> getSNCLs() const noexcept;
    /*!
     * @brief Extracts a trace with given SNCL from the miniSEED archive.
     * @param[in] sncl  The SNCL to extract from the archive.
     * @result The trace corresponding to the given SNCL.
     * @throws std::invalid_argument if the SNCL does not exist in the archive.
     * @sa \c haveSNCL()
     */
    Trace getTrace(const SNCL &sncl) const;
    /*!
     * @brief Checks if a trace with the given SNCL exists in the archive.
     * @result True indicates that the SNCL exists in the archive.
     */
    bool haveSNCL(const SNCL &sncl) const noexcept;

    /*!
     * @brief Gets the number of traces.
     * @result The number of traces in the miniseed archive.
     */
    int getNumberOfTraces() const noexcept;

    /*!
     * @brief Gets the earliest start time in the archive.
     */
    Utilities::Time getEarliestStartTime() const;
    Utilities::Time getLatestEndTime() const;

private:
    class TraceGroupImpl;
    std::unique_ptr<TraceGroupImpl> pImpl;
};
}
#endif
