#pragma once

//
// platform_defs_posix.h - Useful platform-dependent macros and utilities for POSIX.
//
// Written by Ryan Antkowiak (antkowiak@gmail.com)
//
// 2020-05-08
//

#if defined(CURRENT_PLATFORM_POSIX)

#include <algorithm>
#include <cerrno>
#include <exception>
#include <map>
#include <sstream>
#include <string>
#include <utility>

namespace rda
{
    namespace platform_defs
    {
        // a posix error number that is set to "errno"
        class posix_error
        {
        public:
            // the error number (errno)
            int error_num = 0;

            // the name of the macro defined for this error number
            std::string error_name;

            // the description of this error number
            std::string error_description;

        private:
            // a map containing all errno numbers and their meaning
            static std::map<int, std::pair<std::string, std::string>> POSIX_ERROR_MAP;

        public:
            // construct a posix_error object
            posix_error(const int error_num_)
                : error_num(error_num_),
                  error_name(""),
                  error_description("Unknown error")
            {
                // initialize the static  map of error numbers if necessary
                init_map();

                // see if the error number is defined in the map
                auto iter = POSIX_ERROR_MAP.find(error_num);

                // copy the name and description of the error to this object
                if (iter != POSIX_ERROR_MAP.end())
                {
                    this->error_name = iter->second.first;
                    this->error_description = iter->second.second;
                }
            }

            // check if equals another posix_error object (compares errno number)
            bool operator==(const posix_error &rhs) const
            {
                return (error_num == rhs.error_num);
            }

            // check if this posix_error object is for the given errno number
            bool operator==(const int rhs_err_num) const
            {
                return (error_num == rhs_err_num);
            }

        private:
            // static method to populate the map of errno error definitions
            static void init_map()
            {
                if (POSIX_ERROR_MAP.empty())
                {
                    POSIX_ERROR_MAP = {
                        {EPERM, {"EPERM", "Operation not permitted"}},
                        {ENOENT, {"ENOENT", "No such file or directory"}},
                        {ESRCH, {"ESRCH", "No such process"}},
                        {EINTR, {"EINTR", "Interrupted system call"}},
                        {EIO, {"EIO", "I/O error"}},
                        {ENXIO, {"ENXIO", "No such device or address"}},
                        {E2BIG, {"E2BIG", "Argument list too long"}},
                        {ENOEXEC, {"ENOEXEC", "Exec format error"}},
                        {EBADF, {"EBADF", "Bad file number"}},
                        {ECHILD, {"ECHILD", "No child processes"}},
                        {EAGAIN, {"EAGAIN", "Try again"}},
                        {ENOMEM, {"ENOMEM", "Out of memory"}},
                        {EACCES, {"EACCES", "Permission denied"}},
                        {EFAULT, {"EFAULT", "Bad address"}},
                        {ENOTBLK, {"ENOTBLK", "Block device required"}},
                        {EBUSY, {"EBUSY", "Device or resource busy"}},
                        {EEXIST, {"EEXIST", "File exists"}},
                        {EXDEV, {"EXDEV", "Cross-device link"}},
                        {ENODEV, {"ENODEV", "No such device"}},
                        {ENOTDIR, {"ENOTDIR", "Not a directory"}},
                        {EISDIR, {"EISDIR", "Is a directory"}},
                        {EINVAL, {"EINVAL", "Invalid argument"}},
                        {ENFILE, {"ENFILE", "File table overflow"}},
                        {EMFILE, {"EMFILE", "Too many open files"}},
                        {ENOTTY, {"ENOTTY", "Not a typewriter"}},
                        {ETXTBSY, {"ETXTBSY", "Text file busy"}},
                        {EFBIG, {"EFBIG", "File too large"}},
                        {ENOSPC, {"ENOSPC", "No space left on device"}},
                        {ESPIPE, {"ESPIPE", "Illegal seek"}},
                        {EROFS, {"EROFS", "Read-only file system"}},
                        {EMLINK, {"EMLINK", "Too many links"}},
                        {EPIPE, {"EPIPE", "Broken pipe"}},
                        {EDOM, {"EDOM", "Math argument out of domain of func"}},
                        {ERANGE, {"ERANGE", "Math result not representable"}},
                        {EDEADLK, {"EDEADLK", "Resource deadlock would occur"}},
                        {ENAMETOOLONG, {"ENAMETOOLONG", "File name too long"}},
                        {ENOLCK, {"ENOLCK", "No record locks available"}},
                        {ENOSYS, {"ENOSYS", "Invalid system call number"}},
                        {ENOTEMPTY, {"ENOTEMPTY", "Directory not empty"}},
                        {ELOOP, {"ELOOP", "Too many symbolic links encountered"}},
                        {ENOMSG, {"ENOMSG", "No message of desired type"}},
                        {EIDRM, {"EIDRM", "Identifier removed"}},
                        {ECHRNG, {"ECHRNG", "Channel number out of range"}},
                        {EL2NSYNC, {"EL2NSYNC", "Level 2 not synchronized"}},
                        {EL3HLT, {"EL3HLT", "Level 3 halted"}},
                        {EL3RST, {"EL3RST", "Level 3 reset"}},
                        {ELNRNG, {"ELNRNG", "Link number out of range"}},
                        {EUNATCH, {"EUNATCH", "Protocol driver not attached"}},
                        {ENOCSI, {"ENOCSI", "No CSI structure available"}},
                        {EL2HLT, {"EL2HLT", "Level 2 halted"}},
                        {EBADE, {"EBADE", "Invalid exchange"}},
                        {EBADR, {"EBADR", "Invalid request descriptor"}},
                        {EXFULL, {"EXFULL", "Exchange full"}},
                        {ENOANO, {"ENOANO", "No anode"}},
                        {EBADRQC, {"EBADRQC", "Invalid request code"}},
                        {EBADSLT, {"EBADSLT", "Invalid slot"}},
                        {EBFONT, {"EBFONT", "Bad font file format"}},
                        {ENOSTR, {"ENOSTR", "Device not a stream"}},
                        {ENODATA, {"ENODATA", "No data available"}},
                        {ETIME, {"ETIME", "Timer expired"}},
                        {ENOSR, {"ENOSR", "Out of streams resources"}},
                        {ENONET, {"ENONET", "Machine is not on the network"}},
                        {ENOPKG, {"ENOPKG", "Package not installed"}},
                        {EREMOTE, {"EREMOTE", "Object is remote"}},
                        {ENOLINK, {"ENOLINK", "Link has been severed"}},
                        {EADV, {"EADV", "Advertise error"}},
                        {ESRMNT, {"ESRMNT", "Srmount error"}},
                        {ECOMM, {"ECOMM", "Communication error on send"}},
                        {EPROTO, {"EPROTO", "Protocol error"}},
                        {EMULTIHOP, {"EMULTIHOP", "Multihop attempted"}},
                        {EDOTDOT, {"EDOTDOT", "RFS specific error"}},
                        {EBADMSG, {"EBADMSG", "Not a data message"}},
                        {EOVERFLOW, {"EOVERFLOW", "Value too large for defined data type"}},
                        {ENOTUNIQ, {"ENOTUNIQ", "Name not unique on network"}},
                        {EBADFD, {"EBADFD", "File descriptor in bad state"}},
                        {EREMCHG, {"EREMCHG", "Remote address changed"}},
                        {ELIBACC, {"ELIBACC", "Can not access a needed shared library"}},
                        {ELIBBAD, {"ELIBBAD", "Accessing a corrupted shared library"}},
                        {ELIBSCN, {"ELIBSCN", ".lib section in a.out corrupted"}},
                        {ELIBMAX, {"ELIBMAX", "Attempting to link in too many shared libraries"}},
                        {ELIBEXEC, {"ELIBEXEC", "Cannot exec a shared library directly"}},
                        {EILSEQ, {"EILSEQ", "Illegal byte sequence"}},
                        {ERESTART, {"ERESTART", "Interrupted system call should be restarted"}},
                        {ESTRPIPE, {"ESTRPIPE", "Streams pipe error"}},
                        {EUSERS, {"EUSERS", "Too many users"}},
                        {ENOTSOCK, {"ENOTSOCK", "Socket operation on non-socket"}},
                        {EDESTADDRREQ, {"EDESTADDRREQ", "Destination address required"}},
                        {EMSGSIZE, {"EMSGSIZE", "Message too long"}},
                        {EPROTOTYPE, {"EPROTOTYPE", "Protocol wrong type for socket"}},
                        {ENOPROTOOPT, {"ENOPROTOOPT", "Protocol not available"}},
                        {EPROTONOSUPPORT, {"EPROTONOSUPPORT", "Protocol not supported"}},
                        {ESOCKTNOSUPPORT, {"ESOCKTNOSUPPORT", "Socket type not supported"}},
                        {EOPNOTSUPP, {"EOPNOTSUPP", "Operation not supported on transport endpoint"}},
                        {EPFNOSUPPORT, {"EPFNOSUPPORT", "Protocol family not supported"}},
                        {EAFNOSUPPORT, {"EAFNOSUPPORT", "Address family not supported by protocol"}},
                        {EADDRINUSE, {"EADDRINUSE", "Address already in use"}},
                        {EADDRNOTAVAIL, {"EADDRNOTAVAIL", "Cannot assign requested address"}},
                        {ENETDOWN, {"ENETDOWN", "Network is down"}},
                        {ENETUNREACH, {"ENETUNREACH", "Network is unreachable"}},
                        {ENETRESET, {"ENETRESET", "Network dropped connection because of reset"}},
                        {ECONNABORTED, {"ECONNABORTED", "Software caused connection abort"}},
                        {ECONNRESET, {"ECONNRESET", "Connection reset by peer"}},
                        {ENOBUFS, {"ENOBUFS", "No buffer space available"}},
                        {EISCONN, {"EISCONN", "Transport endpoint is already connected"}},
                        {ENOTCONN, {"ENOTCONN", "Transport endpoint is not connected"}},
                        {ESHUTDOWN, {"ESHUTDOWN", "Cannot send after transport endpoint shutdown"}},
                        {ETOOMANYREFS, {"ETOOMANYREFS", "Too many references: cannot splice"}},
                        {ETIMEDOUT, {"ETIMEDOUT", "Connection timed out"}},
                        {ECONNREFUSED, {"ECONNREFUSED", "Connection refused"}},
                        {EHOSTDOWN, {"EHOSTDOWN", "Host is down"}},
                        {EHOSTUNREACH, {"EHOSTUNREACH", "No route to host"}},
                        {EALREADY, {"EALREADY", "Operation already in progress"}},
                        {EINPROGRESS, {"EINPROGRESS", "Operation now in progress"}},
                        {ESTALE, {"ESTALE", "Stale file handle"}},
                        {EUCLEAN, {"EUCLEAN", "Structure needs cleaning"}},
                        {ENOTNAM, {"ENOTNAM", "Not a XENIX named type file"}},
                        {ENAVAIL, {"ENAVAIL", "No XENIX semaphores available"}},
                        {EISNAM, {"EISNAM", "Is a named type file"}},
                        {EREMOTEIO, {"EREMOTEIO", "Remote I/O error"}},
                        {EDQUOT, {"EDQUOT", "Quota exceeded"}},
                        {ENOMEDIUM, {"ENOMEDIUM", "No medium found"}},
                        {EMEDIUMTYPE, {"EMEDIUMTYPE", "Wrong medium type"}},
                        {ECANCELED, {"ECANCELED", "Operation Canceled"}},
                        {ENOKEY, {"ENOKEY", "Required key not available"}},
                        {EKEYEXPIRED, {"EKEYEXPIRED", "Key has expired"}},
                        {EKEYREVOKED, {"EKEYREVOKED", "Key has been revoked"}},
                        {EKEYREJECTED, {"EKEYREJECTED", "Key was rejected by service"}}}; // POSIX_ERROR_MAP
                }
            }
        }; // class posix_error

        // create an exception based on a posix errno code
        class posix_exception : public std::exception
        {
        public:
            // the system call that generated this exception
            std::string syscall_name;

            // the posix error object for the errno code
            posix_error err;

            // constructor
            posix_exception(std::string syscall_name_, const int error_num)
                : syscall_name(std::move(syscall_name_)),
                  err(error_num)
            {
            }

            // returns the error message for this exception
            virtual std::string what_str() const
            {
                std::stringstream ss;
                ss << "Exception: " << syscall_name;
                ss << "() errno=" << err.error_num;
                ss << " [" << err.error_name << "] ";
                ss << err.error_description;
                return ss.str();
            }
        }; // class posix_exception

    } // namespace platform_defs
} // namespace rda

#endif
