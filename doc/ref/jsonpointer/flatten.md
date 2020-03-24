### jsoncons::jsonpointer::flatten

```c++
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>

    template<class Json>
    Json flatten(const Json& value)
```
Flattens a json object or array into a single depth object of key-value pairs.
The keys in the flattened object are JSONPointer's.
The values are primitive (string, number, boolean, or null). Empty objects or arrays become null.

#### Return value

A flattened json object of JSONPointer-value pairs

### Examples

#### Flatten

```c++
#include <jsoncons/json.hpp>
#include <jsoncons_ext/jsonpointer/jsonpointer.hpp>

int main()
{
    json input = json::parse(R"(
    {
       "application": "hiking",
       "reputons": [
           {
               "rater": "HikingAsylum",
               "assertion": "advanced",
               "rated": "Marilyn C",
               "rating": 0.90
            },
           {
               "rater": "HikingAsylum",
               "assertion": "intermediate",
               "rated": "Hongmin",
               "rating": 0.75
            }       
        ]
    }
    )");

    json result = jsonpointer::flatten(input);

    std::cout << pretty_print(result) << "\n";
}
```
Output:
```
{
    "/application": "hiking",
    "/reputons/0/assertion": "advanced",
    "/reputons/0/rated": "Marilyn C",
    "/reputons/0/rater": "HikingAsylum",
    "/reputons/0/rating": 0.9,
    "/reputons/1/assertion": "intermediate",
    "/reputons/1/rated": "Hongmin",
    "/reputons/1/rater": "HikingAsylum",
    "/reputons/1/rating": 0.75
}
```
### See also

[jsoncons::jsonpath::flatten](../jsonpath/flatten.md)