namespace Test_Deferred {
int test_deferred();
}
namespace Test_Physics {
int test_physics();
}

int main(int /* argc */, const char* /* argv */[]) {
    // return Test_Physics::test_physics();
    return Test_Deferred::test_deferred();
}
