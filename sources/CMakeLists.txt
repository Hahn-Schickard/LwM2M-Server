cmake_minimum_required(VERSION 3.6) 

# Binding_Interface implementers must use this macro to expand the bindings list
FUNCTION(ADD_NEW_BINDING type)
    set_property(GLOBAL APPEND PROPERTY Bindings ${type})
ENDFUNCTION()

add_subdirectory(Messages)
add_subdirectory(Device_Model)
add_subdirectory(XML_Parser)
add_subdirectory(Device_Registry)
add_subdirectory(Binding_Interface)
add_subdirectory(CoAP_Binding)
add_subdirectory(Server)
add_subdirectory(Example)