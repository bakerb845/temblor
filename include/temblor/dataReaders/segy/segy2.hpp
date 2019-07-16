#ifndef TEMBLOR_DATAREADERS_SEGY_SEGY2_HPP
#define TEMBLOR_DATAREADERS_SEGY_SEGY2_HPP
#include <memory>
#include <string>
#include <vector>

namespace Temblor::DataReaders::SEGY
{

/*!
 * @brief The SEGY-2 class.
 */
class Segy2
{
public:
    /*! @name Constructors
     * *@{
     */
    /*! 
     * @brief Constructor.
     */
    Segy2();
    /*! @} */

    /*! @name Destructors
     * @{
     */
    /*!
     * @brief Default destructor.
     */
    ~Segy2();
    /*!
     * @brief Clears all data.
     */
    void clear() noexcept;
    /*! @} */ 

    /*! @name Readers and Writers
     * @{
     */ 
    /*!
     * @brief Reads a SEGY-2 file.
     * @param[in] fileName   The name of the SEGY-2 file to read.
     * @throws std::invalid_argument if the fileName does not exist or refers
     *         to an invalid SEGY-2 file.
     */
    void read(const std::string &fileName);

    /*! @} */

    /*!
     * @brief Gets the 3200 character textual header. 
     * @result The 3200 character textual header.
     * @note This will be 3200 blank spaces if the header has yet to be set.
     */
    std::string getTextualHeader() const noexcept;
private:
    class Segy2Impl;
    std::unique_ptr<Segy2Impl> pImpl;
};

}

#endif
