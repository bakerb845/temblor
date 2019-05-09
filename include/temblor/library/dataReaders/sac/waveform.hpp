#ifndef TEMBLOR_LIBRARY_DATAREADERS_SAC_WAVEFORM_HPP
#define TEMBLOR_LIBRARY_DATAREADERS_SAC_WAVEFORM_HPP
#include <memory>
#include <string>
#include "temblor/library/dataReaders/sac/enums.hpp"

namespace Temblor::Library::DataReaders::SAC
{

/*!
 * @brief The SAC waveform class.  A SAC waveform is comprised of a time series
 *        and is characterized by a SAC header.
 */
class Waveform
{
public:
    /*! @name Constructors
     * *@{
     */
    /*!
     * @brief Constructor.
     */
    Waveform();
    /*!
     * @brief Copy constructor.
     * @param[in] waveform  The waveform class from which to
     *                      construct this class.
     */
    Waveform(const Waveform &waveform);
    /*!
     * @brief Move constructor.
     * @param[in,out] waveform  The waveform class to move to this class.
     *                          On exit, waveform's behavior will be undefined.
     */
    Waveform(Waveform &&waveform);
    /*! @} */

    /*! @name Operators
     * @{
     */
    /*!
     * @brief Copy assignment operator.
     * @param[in] waveform  The waveform class to copy.
     * @result A deep copy of the input waveform.
     */
    Waveform& operator=(const Waveform &waveform);
    /*!
     * @brief Move assignment operator.
     * @param[in,out] waveform  The waveform to move.  On exit waveform's
     *                          behavior will be undefined.
     */
    Waveform& operator=(Waveform &&waveform);
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Destructor.
     */
    ~Waveform();
    /*! @} */

    /*!
     * @brief Checks if this class is a valid representation of a SAC file.
     *        This means that the header is defined and that there is data.
     * @result True indicates that this class represents a valid SAC waveform. 
     */
    bool isValid() noexcept;
    /*!
     * @brief Loads a SAC data file.
     * @param[in] fileName  The name of file to read.
     * @throws std::invalid_argument if fileName does not exist
     *         or the SAC file is unreadable.
     */
    void read(const std::string &fileName);
    /*!
     * @brief Writes the SAC file.
     * @param[out] fileName  The SAC file to write.
     * @throws std::invalid_argument if the path to fileName is invalid.
     * @throws std::runtime_error if the SAC class is valid.
     */
    void write(const std::string &fileName); 
private:
    class WaveformImpl;
    std::unique_ptr<WaveformImpl> pImpl;
};

}
#endif
