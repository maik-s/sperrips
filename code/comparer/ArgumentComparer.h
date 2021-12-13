# ifndef MSC_ARGUMENT_COMPARER_H
# define MSC_ARGUMENT_COMPARER_H

template<class T>
/**
 * @brief Base class for comparer implementations.
 *
 */
class ArgumentComparer {
private:
    int verbosityLevel = 0;
public:

    /**
     * @brief Compares two arguments.
     *
     * @param recorded Argument value as retrieved in the recording phase.
     * @param replayed Argument value as retrieved in the replaying phase.
     * @return true Arguments are equal regarding the implemented logic.
     * @return false Arguments are unequal regarding the implemented logic.
     */
    virtual bool compare(T recorded, T replayed) = 0;
};
#endif