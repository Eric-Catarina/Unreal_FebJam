#include <iostream>
#include <optional>
#include <stdexcept>
#include <variant>

template <typename T, typename E>
class Result {
public:
    static Result<T, E> Ok(T value) {
        return Result<T, E>(std::move(value), std::nullopt);
    }

    static Result<T, E> Err(E error) {
        return Result<T, E>(std::nullopt, std::move(error));
    }

    UFUNCTION(BlueprintPure, Category = "Result")

    bool IsOk() const {
        return Value.has_value();
    }

    UFUNCTION(BlueprintPure, Category = "Result")

    bool IsError() const {
        return ErrorValue.has_value();
    }

    UFUNCTION(BlueprintPure, Category = "Result")

    T &Unwrap() {
        if (IsError()) {
            throw std::runtime_error("Called unwrap on an Err result");
        }
        return *Value;
    }

    UFUNCTION(BlueprintPure, Category = "Result")

    E &UnwrapError() {
        if (IsOk()) {
            throw std::runtime_error("Called unwrap_err on an Ok result");
        }

        return *ErrorValue;
    }

    UFUNCTION(BlueprintCallable, Category = "Result")

    T UnrwapOr(T default_value) const {
        return IsOk() ? *Value : default_value;
    }

    template <typename Func>
    T UnwrapOrElse(Func &&fallback_func) const {
        return IsOk() ? *Value : fallback_func();
    }

private:
    std::optional<T> Value;
    std::optional<E> ErrorValue;

    Result(std::optional<T> value, std::optional<E> error)
        : Value(std::move(value)), ErrorValue(std::move(error)) {
    }
};
