#pragma once

#include <memory>
#include <string>

/// @cond INTERNAL
namespace spdlog
{
    class logger;
}
/// @endcond

namespace qf
{
    /**
     * @brief Thread-safe singleton logger backed by spdlog.
     * @ingroup qf_classes
     *
     * Wraps an spdlog stdout logger and exposes a simplified interface for
     * emitting messages at the standard severity levels.  Only one instance
     * exists per process; obtain it via `logger::get()`.
     *
     * @par Example
     * @code{.cpp}
     * qf::logger::get().set_level(qf::logger::level::DEBUG);
     * qf::logger::get().debug("starting computation");
     * @endcode
     *
     * @note The logger is non-copyable and non-movable.
     */
    class logger
    {
      public:
        /**
         * @brief Severity levels available to the logger.
         *
         * The numeric values mirror spdlog's own `level_enum`, allowing
         * simple integer comparisons between the two.
         */
        enum class level
        {
            TRACE = 0,    ///< Fine-grained diagnostic messages.
            DEBUG = 1,    ///< Coarse-grained diagnostic messages.
            INFO = 2,     ///< Informational runtime messages.
            WARN = 3,     ///< Potentially harmful situations.
            ERR = 4,      ///< Error events that may still allow execution.
            CRITICAL = 5, ///< Severe errors that will likely abort the program.
            OFF = 6,      ///< Disables all logging output.
        };

        /**
         * @brief Returns the process-wide logger instance.
         *
         * The instance is constructed on first call (Meyer's singleton) and
         * initialises the underlying spdlog logger with level `OFF`.
         *
         * @return Reference to the singleton `logger` object.
         */
        static logger& get();

        /**
         * @brief Sets the minimum severity level for emitted messages.
         *
         * Messages below @p log_level are silently discarded.
         *
         * @param log_level The desired minimum severity level.
         */
        void set_level(level log_level);

        /**
         * @brief Returns the currently active severity level.
         *
         * @return The current `level` value.
         */
        level get_level() const;

        /**
         * @brief Emits a message at `TRACE` severity.
         * @param msg The message text.
         */
        void trace(const std::string& msg) const;

        /**
         * @brief Emits a message at `DEBUG` severity.
         * @param msg The message text.
         */
        void debug(const std::string& msg) const;

        /**
         * @brief Emits a message at `INFO` severity.
         * @param msg The message text.
         */
        void info(const std::string& msg) const;

        /**
         * @brief Emits a message at `WARN` severity.
         * @param msg The message text.
         */
        void warn(const std::string& msg) const;

        /**
         * @brief Emits a message at `ERR` severity.
         * @param msg The message text.
         */
        void error(const std::string& msg) const;

        /**
         * @brief Emits a message at `CRITICAL` severity.
         * @param msg The message text.
         */
        void critical(const std::string& msg) const;

        /// @cond INTERNAL
        logger(logger&) = delete;
        logger& operator=(logger&) = delete;
        logger(logger&&) = delete;
        logger&& operator=(logger&&) = delete;
        /// @endcond

      private:
        /** @brief Constructs the singleton; called once by `get()`. */
        logger();

        std::shared_ptr<spdlog::logger> m_logger; ///< Underlying spdlog logger.
        level m_level;                            ///< Currently active severity level.
    };
} // namespace qf
