#[Lambda calculus](https://en.wikipedia.org/wiki/Lambda_calculus) interpreter   
##syntax    
    <expr> ::= <identifier>      
    <expr> ::= "(" "lambda" <identifier> <expr> ")"   
    <expr> ::= "(" <expr> <expr> ")"    
##example   
    //FACT(3):
    >>>(λ f. ((λ x. f (x x)) (λ x. f (x x)))) (λ g. λ n. n (λ u. (λ m. λ n. λ f. m (n f)) n (g ((λ n. λ f. λ x. n (λ g. λ h. h (g f)) (λ u. x) (λ u. u)) n))) (λ f. λ x. f x)) (λ f. λ x. f (f (f x)))     
    (λ f. (λ x. (f (f (f (f (f (f x))))))))

    //Consider that the character 'λ' is hard to type for some keyboard, you can use the word "lambda" instead, hence the same example can be:
    >>>(lambda f. ((lambda x. f (x x)) (lambda x. f (x x)))) (lambda g. lambda n. n (lambda u. (lambda m. lambda n. lambda f. m (n f)) n (g ((lambda n. lambda f. lambda x. n (lambda g. lambda h. h (g f)) (lambda u. x) (lambda u. u)) n))) (lambda f. lambda x. f x)) (lambda f. lambda x. f (f (f x)))     
    (λ f. (λ x. (f (f (f (f (f (f x))))))))
