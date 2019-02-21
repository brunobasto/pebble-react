Simple and low fat json parser

I was looking for one that could be used on pebble to process responses from the websites in C rather than js. The idea is to pass to pebble the json string instead of each value, and process it locally. This is useful if more data needs to be passed.

It takes a null terminated json string and calls the callback functions on each object and array value.

It does not create additional buffers, but returns the pointers to the input json string, so requires minimum memory (just some variables and recursive call stack with depth depending on the depth of the json).

The callback will be issued also if the value is an array or object, so the callback function needs to check the value type. After the callback is processed, the parser will continue, so in case there is an json as foloows:

```
{
  "name":"test",
  "num":5,
  "array":[
    1,
    2
  ],
  "object":{
    "type":"some type",
    "value":"some value"
  },
  "array_of_object":[
    {
      "name":"john",
      "age":20
    },
    {
      "name":"anna",
      "age":21
    }
  ],
  "object_with_array":{
    "team":[
      "adam",
      "tom",
      "moni"
    ],
    "strangers":[
      "kathy",
      "rolf"
    ]
  }
}
```

The following callbacks will be issued:

```
object_callback for type    JSP_VALUE_STRING -> name : "test"
object_callback for type JSP_VALUE_PRIMITIVE -> num : 5
object_callback for type     JSP_VALUE_ARRAY -> array : [1, 2 ]
 array_callback for type JSP_VALUE_PRIMITIVE -> 1
 array_callback for type JSP_VALUE_PRIMITIVE -> 2
object_callback for type    JSP_VALUE_OBJECT -> object : {"type":"some type", "value":"some value"}
object_callback for type    JSP_VALUE_STRING -> type : "some type"
object_callback for type    JSP_VALUE_STRING -> value : "some value"
object_callback for type     JSP_VALUE_ARRAY -> array_of_object : [{"name":"john", "age":20}, {"name":"anna", "age":21}
object_callback for type    JSP_VALUE_STRING -> name : "john"
object_callback for type JSP_VALUE_PRIMITIVE -> age : 20
object_callback for type    JSP_VALUE_STRING -> name : "anna"
object_callback for type JSP_VALUE_PRIMITIVE -> age : 21
object_callback for type    JSP_VALUE_OBJECT -> object_with_array : {"team":["adam", "tom", "moni"], "strangers":["kathy", "rolf"]}
object_callback for type     JSP_VALUE_ARRAY -> team : ["adam", "tom", "moni"]
 array_callback for type    JSP_VALUE_STRING -> "adam"
 array_callback for type    JSP_VALUE_STRING -> "tom"
 array_callback for type    JSP_VALUE_STRING -> "moni"
object_callback for type     JSP_VALUE_ARRAY -> strangers : ["kathy", "rolf"]
 array_callback for type    JSP_VALUE_STRING -> "kathy"
 array_callback for type    JSP_VALUE_STRING -> "rolf"
```

Enjoy.