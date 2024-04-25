#include <stdlib.h>
#include <string.h>

//dynamic array
typedef struct Array{
    u_int64_t array_length;
    const char * array_pointer;
    const u_int64_t capacity;
    const char * null_frame;
    const u_int8_t null_frame_size;
}array;



static array * grow(

        const array * array_p,
        const u_int64_t new_capacity

        )
{

    u_int64_t _capacity = array_p -> capacity;
    const char * _null_frame = array_p -> null_frame;
    const u_int8_t _null_frame_size = array_p -> null_frame_size;

    const char * _new_pointer = malloc((array_p -> null_frame_size)*new_capacity);

    memcpy(_new_pointer, array_p -> array_pointer, (array_p -> null_frame_size)*(array_p -> array_length));

    while(new_capacity-_capacity>0){
        memcpy(_new_pointer+_capacity * _null_frame_size, _null_frame, _null_frame_size);
        ++_capacity;
    }

    array new_array = {
            .array_length = array_p -> array_length,
            .array_pointer = _new_pointer,
            .capacity = new_capacity,
            .null_frame = array_p -> null_frame,
            .null_frame_size = array_p -> null_frame_size
    };

    const char * pointer = malloc(sizeof(new_array));
    memcpy(pointer, &new_array, sizeof(new_array));

    return (array *) pointer;
}



typedef char * (*unary_operation)(const char *, const u_int8_t, u_int8_t *);



static array * map(

        const array * array_p,
        const unary_operation operation

        )
{

    u_int64_t _array_length = array_p -> array_length;
    const u_int8_t _null_frame_size = array_p -> null_frame_size;

    const char * _old_pointer = malloc((array_p -> null_frame_size)*(array_p -> capacity));

    memcpy(_old_pointer, array_p -> array_pointer, (array_p -> null_frame_size)*(array_p -> array_length));

    const u_int8_t _new_frame_size1;
    const u_int8_t _new_frame_size2;

    operation(_old_pointer, _null_frame_size, &_new_frame_size1);

    if(_new_frame_size1 <1 )
    {
        exit(0);
    }

    const char * _new_pointer = malloc((_new_frame_size1)*(array_p -> capacity));

    const char * result;
    u_int64_t _new_array_length = 0;

    char * __old_pointer = _old_pointer;

    while(_array_length > 0)
    {
        result = operation(__old_pointer, _null_frame_size, &_new_frame_size2);
        if(_new_frame_size2 != _new_frame_size1)
        {
            exit(0);
        }
        memcpy(_new_pointer+_new_array_length * _new_frame_size1, result, _new_frame_size1);
        --_array_length;
        ++_new_array_length;
        __old_pointer += _new_frame_size1;
    }

    const array new_array = {
            .array_length = array_p -> array_length,
            .array_pointer = _new_pointer,
            .capacity = array_p -> capacity,
            .null_frame = array_p -> null_frame,
            .null_frame_size = _new_frame_size1
    };

    const char * pointer = malloc(sizeof(new_array));
    memcpy(pointer, &new_array, sizeof(new_array));

    return (array *) pointer;
}



typedef int (*unary_condition)(const char *, const u_int8_t);



static array * where(

        const array * array_p,
        const unary_condition condition

        )
{

    u_int64_t _new_length = 0;
    u_int64_t _old_length = array_p -> array_length;
    const u_int8_t _null_frame_size = array_p -> null_frame_size;

    const char * _new_pointer = malloc((array_p -> null_frame_size)*(array_p -> array_length));

    const char * _old_pointer = malloc((array_p -> null_frame_size)*(array_p -> array_length));

    memcpy(_old_pointer, array_p -> array_pointer, (array_p -> null_frame_size)*(array_p -> array_length));

    while(_old_length>0)
    {
        if(condition(_old_pointer, _null_frame_size))
        {
            memcpy(_new_pointer + _new_length * _null_frame_size, _old_pointer, _null_frame_size);
            ++_new_length;
        }
        --_old_length;
        _old_pointer += _null_frame_size;
    }

    array new_array = {
            .array_length = _new_length,
            .array_pointer = _new_pointer,
            .capacity = array_p -> capacity,
            .null_frame = array_p -> null_frame,
            .null_frame_size = array_p -> null_frame_size
    };

    char * pointer = malloc(sizeof(new_array));
    memcpy(pointer, &new_array, sizeof(new_array));

    return (array *) pointer;
}



static void free_array(

        const array * array_p

        )
{

    free(array_p -> array_pointer);
    free(array_p);

}



static array * add_all(

        const array * array_first,
        const array * array_second

)
{

    const u_int8_t _null_frame_size = array_first -> null_frame_size;
    const u_int64_t _length_first = array_first -> array_length;
    const u_int64_t _length_second = array_second -> array_length;


    if(_null_frame_size != array_second -> null_frame_size)
    {
        exit(0);
    }

    char * _new_pointer = malloc(_null_frame_size * (_length_first + _length_second));

    memcpy(_new_pointer, array_first -> array_pointer, _length_first * _null_frame_size);
    memcpy(_new_pointer + _length_first * _null_frame_size, array_second -> array_pointer, _length_second * _null_frame_size);


    array new_array = {
            .array_length = _length_first+_length_second,
            .array_pointer = _new_pointer,
            .capacity = array_first -> capacity + array_second -> capacity,
            .null_frame = array_first -> null_frame,
            .null_frame_size = _null_frame_size
    };

    char * pointer = malloc(sizeof(new_array));
    memcpy(pointer, &new_array, sizeof(new_array));

    return (array *) pointer;
}




static array * add(

        const array * array_p,
        const char * el_p

        )
{

    const array * new_array;

    const u_int64_t _capacity = array_p -> capacity;
    const u_int64_t _length = array_p -> array_length;
    const u_int8_t _null_frame_size = array_p -> null_frame_size;

    if(_capacity < _length + 1)
    {
        new_array = grow(array_p, array_p -> capacity * 2);
    }
    else
    {
        new_array = malloc(sizeof(array));
        memcpy(new_array, array_p, sizeof(array));
    }
    const char * _array_pointer = new_array -> array_pointer;

    memcpy(_array_pointer + _length * _null_frame_size, el_p, _null_frame_size);

    return new_array;
}



static char * get(

        const array * array_p,
        const int index

        )
{

    const char * element = array_p -> array_pointer + index * array_p -> null_frame_size;

    return element;
}



static array * delete(

        const array * array_p,
        const int index

        )
{

    const array * new_array;

    const u_int64_t _capacity = array_p -> capacity;
    const u_int64_t _length = array_p -> array_length;
    const u_int8_t _null_frame_size = array_p -> null_frame_size;
    const char * _pointer = array_p -> array_pointer;

    new_array = malloc(sizeof(array));
    memcpy(new_array, array_p, sizeof(array));

    memcpy(new_array -> array_pointer, _pointer, _null_frame_size * (index - 1));

    const u_int8_t _len = (index+1) * _null_frame_size;

    memcpy(new_array -> array_pointer + _len, _pointer + _len, _null_frame_size * (_length - index - 1));

    return new_array;
}



static array * create_array(

        const u_int64_t array_length,
        const u_int64_t array_capacity,
        const char * null_frame,
        const u_int8_t null_frame_size

        )
{

    u_int64_t _old_length = array_length;
    const char * _new_pointer = malloc(null_frame_size * array_capacity);

    while(_old_length > 0){
        memcpy(_new_pointer + (array_length - _old_length) * null_frame_size, null_frame, null_frame_size);
        --_old_length;
    }

    array new_array = {
            .array_length = array_length,
            .array_pointer = _new_pointer,
            .capacity = array_capacity,
            .null_frame = null_frame,
            .null_frame_size = null_frame_size
    };

    char * pointer = malloc(sizeof(new_array));
    memcpy(pointer, &new_array, sizeof(new_array));

    return (array *) pointer;
}



typedef void ( *print )( const char *, const u_int8_t);


static void print_array(

        const array * array_p,
        const print print_output

        )
{
    const char * _pointer = array_p -> array_pointer;
    const u_int8_t _null_frame_size = array_p -> null_frame_size;
    const u_int64_t _length = array_p -> array_length;

    u_int64_t _old_length = array_p -> array_length;

    while(_old_length > 0){
        print_output(_pointer + (_length - _old_length) * _null_frame_size, _null_frame_size);
        --_old_length;
    }

}