-- Find customer
CREATE OR REPLACE PROCEDURE find_customer(customer_id_input IN NUMBER, found OUT NUMBER) IS 

BEGIN
Select count(customer_id) INTO found FROM CUSTOMERS 
WHERE customer_id = customer_id_input;

EXCEPTION
WHEN no_data_found THEN
found := 0;

END;


-- Find product
CREATE OR REPLACE PROCEDURE find_product (product_id_input IN NUMBER, price OUT products.list_price%TYPE) IS 

BEGIN
Select list_price INTO price FROM PRODUCTS 
WHERE product_id = product_id_input;

EXCEPTION
WHEN no_data_found THEN
price := 0;

END;
