# Writing C++ Documentation

The C++ docs are generated with **Doxygen**.

**New joiners:** you only need to write Doxygen comments in your header files.
The build pipeline picks them up automatically — no separate configuration files
to edit.

---

## Quick-start

### 1. Comment your header

Use `///` for single-line comments or `/** … */` for multi-line blocks:

```cpp
/// @brief One-sentence summary shown in the API index.
void my_function();

/**
 * @brief One-sentence summary shown in the API index.
 *
 * Longer explanation goes here.  You can use multiple paragraphs.
 *
 * @param mat   The input matrix.  Eigen::Ref avoids copies.
 * @param scale Scalar multiplier applied to every element.
 * @return      A new matrix with each element multiplied by @p scale.
 *
 * @par Example
 * @code{.cpp}
 * qf::math::matrix A = qf::math::matrix::Identity(3, 3);
 * auto B = scale_matrix(A, 2.0);
 * @endcode
 *
 * @note Behaviour is undefined if @p scale is NaN.
 * @see qf::math::matrix
 */
qf::math::matrix scale_matrix(Eigen::Ref<const qf::math::matrix> mat,
                               double scale);
```

### 2. Place the symbol in a group (optional)

Groups appear in the left-hand navigation panel.  Annotate with `@ingroup`:

| Group tag        | Where it appears     |
|------------------|----------------------|
| `qf_classes`     | "Classes" section    |
| `qf_functions`   | "Free Functions"     |
| `qf_types`       | "Type Aliases"       |

Groups are defined in [groups.hpp](../lib/inc/quant_finance/groups.hpp).
Add a new `@defgroup` block there when you need a completely new category.

### 3. Build the docs

```bash
# Generate HTML (output: cpp/docs/build/index.html)
just cpp-docs
```

Open `cpp/docs/html/index.html` in a browser to review.

---

## Common Doxygen tags

| Tag                    | Purpose                                             |
|------------------------|-----------------------------------------------------|
| `@brief`               | One-line summary (shown in index tables)            |
| `@param name desc`     | Document a function parameter                       |
| `@return desc`         | Describe the return value                           |
| `@tparam T desc`       | Document a template parameter                       |
| `@throws ExcType desc` | Exceptions that may be thrown                       |
| `@note text`           | Highlighted note box                                |
| `@warning text`        | Highlighted warning box                             |
| `@par Heading`         | Custom paragraph with a bold heading                |
| `@code{.cpp} … @endcode` | Inline code example with syntax highlighting    |
| `@see Symbol`          | Cross-reference to another symbol                   |
| `@ingroup group_id`    | Add symbol to a navigation group                    |
| `@cond INTERNAL … @endcond` | Hide implementation detail from the docs   |
| `@todo text`           | Generates an entry in the TODO list page            |
| `@deprecated text`     | Marks a symbol as deprecated                        |

---

## Documenting a new class

```cpp
namespace qf
{
    /**
     * @brief A short summary of what the class does.
     * @ingroup qf_classes
     *
     * Longer description of design intent, invariants, and usage patterns.
     *
     * @par Thread safety
     * All public methods are thread-safe.
     */
    class my_class
    {
      public:
        /**
         * @brief Construct a new my_class.
         * @param value Initial value.
         */
        explicit my_class(double value);

        /// @brief Returns the stored value.
        [[nodiscard]] double get() const noexcept;
    };
} // namespace qf
```

---

## Documenting a new namespace

Add `///` file-level documentation at the top of your header:

```cpp
/**
 * @file my_module.hpp
 * @brief Public API for the my_module subsystem.
 */

/// @brief Quant Finance root namespace.
namespace qf
{
    ...
}
```

---

## Navigation reference

The generated site offers several ways to find symbols:

| Panel / page       | How to access                              |
|--------------------|--------------------------------------------|
| Left sidebar tree  | Always visible; expands namespaces/classes |
| Full-text search   | Search box in the top-right corner         |
| Class hierarchy    | *Classes → Class Hierarchy* in sidebar     |
| File browser       | *Files* in sidebar                         |
| TODO list          | *Related Pages → Todo List*                |
| Call / caller graphs | Click *Call graph* or *Caller graph* on any function page |
