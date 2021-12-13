# ifndef MSC_HANDLER_H
# define MSC_HANDLER_H

#include <string>
#include <iostream>
#include "../TypeDefs.h"
#include <vector>
#include <sys/syscall.h> //for SYS_ enum is in subclasses
#include "../printers/PrinterRegistry.h"
#include <memory> // for unique pointers
#include <memory> // unique_ptr
#include "../mapper/ASLRMapper.h"
#include "../proto_files/cpp_out/Syscall.pb.h"

class Handler {
public:
    bool _isExitCall = false;
    bool _argumentsDepend = false;
    int _type = -1;
    std::vector<argument_value> _argumentValues = {};
    std::vector<argument_type> _argumentTypes = {};
    argument_value _rval = {};

    /**
     * @brief Indicates whether syscall is an exit syscall.
     *
     * @return true Is an exit syscall.
     * @return false Is not an exit syscall.
     */

    bool isExitCall() {
        return _isExitCall;
    }

    /**
     * @brief Indicates whether syscall has argument that change other argument types, depending on a particular value.
     *
     * @return true Has depending arguments.
     * @return false Has no depending arguments.
     */

    bool hasDependingArguments() {
        return _argumentsDepend;
    }

    /**
     * @brief Get the Argument Values object of the current syscall handler.
     *
     * @return std::vector<argument_value> Saved argument values.
     */

    std::vector<argument_value> getArgumentValues() {
        return _argumentValues;
    }

    /**
     * @brief Get the Return Value object of the current syscall handler.
     *
     * @return argument_value Return value of the syscall.
     */

    argument_value getReturnValue() {
        _rval.type = getReturnType();
        return _rval;
    }

    /**
     * @brief Get the Return Value As Argument object wraped as an argument_value.
     *
     * @return argument_value Return value of the syscall wrapped as an argument_value.
     */

    argument_value getReturnValueAsArgument() {
        argument_value val = _rval;
        val.type = getReturnType();
        return val;
    }

    /**
     * @brief Get the opcode of the current syscall.
     *
     * @return int The syscall's opcode.
     */

    int getType() {
        return _type;
    }

    /**
     * @brief Get the length of a struct (`sizeof(<struct type>)`).
     * The length is determined by the implemented printer of the corresponding struct.
     *
     * @param type Internal representation of the struct, defined in TypeDefs.h
     * @return int Length of the struct.
     */

    int getStructLength(argument_type type) {
        std::unique_ptr<Printer> printer = std::unique_ptr<Printer>(PrinterRegistry<Printer>::create(type));

        if (printer.get() == nullptr) {
            return -1;
        }

        return printer->getStructLength(type);
    }

    /**
     * @brief Updates members of a struct that are of pointer types.
     * The method actually calls `updateStructAdresses()` of a particular `Printer` implementation.
     *
     * @param type Type of struct.
     * @param recorded Struct bytes as recorded in the recording phase.
     * @param replayed Struct bytes as recorded in the replaying phase.
     * @param pointerMapper Reference to pointermapper object.
     * @return std::string Struct bytes with updated addresses.
     */

    std::string updateStructAdresses(argument_type type, std::string recorded, std::string replayed, ASLRMapper* pointerMapper) {
        std::unique_ptr<Printer> printer = std::unique_ptr<Printer>(PrinterRegistry<Printer>::create(type));
        return printer->updateStructAdresses(type, recorded, replayed, pointerMapper);
    }

    /**
     * @brief Set the given grgument values for the current handler.
     *
     * @param argumentValues Argument values.
     */

    void setArgumentValues(std::vector<argument_value> argumentValues) {
        _argumentValues = argumentValues;
    }

    /**
     * @brief Set the Return Value for the current handler.
     *
     * @param returnValue Return value to set.
     */

    void setReturnValue(argument_value returnValue) {
        _rval = returnValue;
    }

    static Handler* create() {
        return new Handler();
    }

    void print();

    /**
     * @brief Get the the types of all arguments of the current syscall handler.
     *
     * @return std::vector<argument_type> Vector of all syscall argument types.
     */

    virtual std::vector<argument_type> getArgumentTypes() {
        return std::vector<argument_type>(0);
    };

    /**
     * @brief Get the return type of a syscall.
     *
     * @return argument_type Syscalls return type.
     */

    virtual argument_type getReturnType() {
        return AT_SIGNED_INT;
    };

    /**
     * @brief Rewrites arguments if the syscall has depending argument. See the corresponding method.
     *
     * @param argumentIndex Index of the argument.
     * @param argumentValue Arguemnt_value struct with new type.
     */

    virtual void rewriteArgumentType(int argumentIndex, argument_value argumentValue) {
    };
};

#endif