#include "SessionClosedException.h"

SessionClosedException::SessionClosedException()
    : AppException("The session is closed.") {
}