void testSpdLog();
void testGLog(char* argv[]);
void testBoostLog();

int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
    testGLog(argv);
    testBoostLog();
    testSpdLog();
}
