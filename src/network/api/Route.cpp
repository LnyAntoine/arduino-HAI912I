#include "Route.h"

Route::Route(const String& path,
             const HTTPMethod method,
             const RouteHandler &handler,
             const String& description)
    : path(path), method(method), handler(handler), description(description) {
}

String Route::getPath() const {
    return path;
}

HTTPMethod Route::getMethod() const {
    return method;
}

RouteHandler Route::getHandler() const {
    return handler;
}

String Route::getDescription() const {
    return description;
}

String Route::getMethodString() const {
    switch (method) {
        case HTTP_GET:     return "GET";
        case HTTP_POST:    return "POST";
        case HTTP_PUT:     return "PUT";
        case HTTP_DELETE:  return "DELETE";
        case HTTP_PATCH:   return "PATCH";
        default:           return "UNKNOWN";
    }
}