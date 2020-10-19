add_executable(
    event_test
    "src/EventTest.cpp"
)

target_link_libraries(event_test PUBLIC gtest_main)
target_link_libraries(event_test PUBLIC cs_events)

add_test(NAME event_test COMMAND event_test)