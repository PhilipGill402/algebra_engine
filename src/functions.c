#include "functions.h"

vector_t functions;

double sin_func(double x) {
    return sin(x);
}

double cos_func(double x) {
    return cos(x);
}

double tan_func(double x) {
    return tan(x);
}

double ln_func(double x) {
    return log(x);
}

double sqrt_func(double x) {
    return sqrt(x);
}

node_t* diff_sin(node_t* self) {
    node_t* node = create_function(string_literal("cos"));
    node->left = node_clone(self->left);
    //node_free(self);

    return node;
}

node_t* diff_cos(node_t* self) {
    node_t* node = create_op('-');
    node->right = create_function(string_literal("sin"));
    node->right->left = node_clone(self->left);
    node->left = create_constant(0);
    //node_free(self);

    return node;
}

node_t* diff_ln(node_t* self) {
    node_t* node = create_op('/');
    node->left = create_constant(1);
    node->right = node_clone(self->left);
    //node_free(self);

    return node;
}

void functions_init() {
    functions = vector_create(sizeof(function_t)); 
    function_t sin_f = { .name = "SIN", .func = sin_func, .diff = diff_sin };
    function_t cos_f = { .name = "COS", .func = cos_func, .diff = diff_cos };
    function_t tan_f = { .name = "TAN", .func = tan_func };
    function_t ln_f = { .name = "LN", .func = ln_func, .diff = diff_ln  };
    function_t sqrt_f = { .name = "SQRT", .func = sqrt_func };
    vector_push_back(&functions, &sin_f);
    vector_push_back(&functions, &cos_f);
    vector_push_back(&functions, &tan_f);
    vector_push_back(&functions, &ln_f);
    vector_push_back(&functions, &sqrt_f);
}
