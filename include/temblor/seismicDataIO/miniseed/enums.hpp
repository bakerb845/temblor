#ifndef TEMBLOR_DATAREADERS_MINISEED_ENUMS_HPP
#define TEMBLOR_DATAREADERS_MINISEED_ENUMS_HPP 1

namespace Temblor::DataReaders::MiniSEED
{
/*!
 * @brief Defines the precision of the data in the trace.
 */
enum class Precision 
{
    INT32,    /*!< 32-bit integer */
    FLOAT32,  /*!< 32-bit floating precision. */
    FLOAT64,  /*!< 64-bit floating precision. */
    UNKNOWN   /*1< An unknown precision. */
};

}

#endif
