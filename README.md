#[Lambda calculus](https://en.wikipedia.org/wiki/Lambda_calculus) interpreter   
##syntax    
    <expr> ::= <identifier>      
    <expr> ::= "(" '\' <identifier> <expr> ")"   
    <expr> ::= "(" <expr> <expr> ")"    
##example   
    //FACT(3):
    >>>(λ f. ((λ x. f (x x)) (λ x. f (x x)))) (λ g. λ n. n (λ u. (λ m. λ n. λ f. m (n f)) n (g ((λ n. λ f. λ x. n (λ g. λ h. h (g f)) (λ u. x) (λ u. u)) n))) (λ f. λ x. f x)) (λ f. λ x. f (f (f x)))     
    (λ f. (λ x. (f (f (f (f (f (f x))))))))

    //Consider that the character 'λ' is hard to type for some keyboard, you can use '\' instead, hence the same example can be:
    >>>(\ f. ((\ x. f (x x)) (\ x. f (x x)))) (\ g. \ n. n (\ u. (\ m. \ n. \ f. m (n f)) n (g ((\ n. \ f. \ x. n (\ g. \ h. h (g f)) (\ u. x) (\ u. u)) n))) (\ f. \ x. f x)) (\ f. \ x. f (f (f x)))     
    (λ f. (λ x. (f (f (f (f (f (f x))))))))
