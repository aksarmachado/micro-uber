#include "protocolo.h"
#include "unity.h"

void setUp(void)
{
// set stuff up here
}

void tearDown(void)
{
// clean stuff up here
}

void test_SeProtocoloMontouMensagemMovVeiculo(void)
{
  /* All of these should pass */
  TEST_ASSERT_EQUAL(variavel, transmite_mov_veiculo(78));
  TEST_ASSERT_EQUAL(4, transmite_mov_veiculo(10));
  TEST_ASSERT_EQUAL(-1, transmite_mov_veiculo(-10));
}

// not needed when using generate_test_runner.rb
int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_SeProtocoloMontouMensagemMovVeiculo);
    
    return UNITY_END();
}

