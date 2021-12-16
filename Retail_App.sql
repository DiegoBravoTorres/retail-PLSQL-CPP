-- ***********************
--
-- Student1 Name: Diego Bravo Torres Student1 ID: 138350202
-- Student2 Name: Victor Lozano Alonso Student2 ID: 130720204
-- Date: Nov 23, 2021
-- Purpose: Assignment 2 - DBS311
--
-- ***********************

SET SERVEROUTPUT ON;

---------- FIND_CUSTOMER -------------

CREATE OR REPLACE PROCEDURE find_customer (cust_id IN NUMBER, found OUT NUMBER) IS
 cid customers.customer_id%TYPE;
 BEGIN
     found:=1;
     SELECT customer_id
     INTO cid
     FROM customers
     WHERE customer_id=cust_id;
     EXCEPTION
     WHEN NO_DATA_FOUND THEN
     found:=0;
 END;
 
 /

---------- FIND_PRODUCT ---------------

CREATE OR REPLACE PROCEDURE find_product(prod_id IN NUMBER, price OUT products.list_price%TYPE) IS
 BEGIN
    SELECT list_price INTO price
    FROM products
    WHERE product_id=prod_id;
        EXCEPTION
          WHEN NO_DATA_FOUND THEN
			price:=0;
 END;
 
/

---------- ADD_ORDER -------------

CREATE OR REPLACE PROCEDURE add_order(cust_id IN NUMBER, new_order_id OUT NUMBER) IS
NUM_OF_ROWS NUMBER;
 BEGIN
        SELECT COUNT(*) INTO NUM_OF_ROWS FROM new_orders;
        -- Check if there's actually any row on that table, if not set the new order id to 1
        IF NUM_OF_ROWS = 0 THEN
        new_order_id:= 1;
        ELSE
        SELECT MAX(order_id) INTO new_order_id
        FROM new_orders;
        new_order_id:=new_order_id+1;
        END IF; 
        
        
        INSERT INTO new_orders
        VALUES(new_order_id, cust_id, 'Shipped', 56, sysdate);
      
 END;

/
---------- ADD_ORDER_ITEM --------

CREATE OR REPLACE PROCEDURE add_order_item(
  orderId IN order_items.order_id%TYPE,
  itemId IN order_items.item_id%TYPE,
  productId IN order_items.product_id%TYPE,
  quantity IN order_items.quantity%TYPE,
  price IN order_items.unit_price%TYPE) IS
    BEGIN
        INSERT INTO new_order_items
        VALUES(orderId, itemId, productId, quantity, price);
    END;
    
    
    
-- For internal use, NOT PART OF SUBMISSION
-- Create new tables

-- CREATE TABLE new_orders
--   (
--     order_id NUMBER PRIMARY KEY,
--     customer_id NUMBER( 6, 0 ) NOT NULL, -- fk
--     status      VARCHAR( 20 ) NOT NULL ,
--     salesman_id NUMBER( 6, 0 )         , -- fk
--     order_date  DATE NOT NULL          ,
--     CONSTRAINT fk_new_orders_customers 
--       FOREIGN KEY( customer_id )
--       REFERENCES customers( customer_id )
--       ON DELETE CASCADE,
--     CONSTRAINT fk_new_orders_employees 
--       FOREIGN KEY( salesman_id )
--       REFERENCES employees( employee_id ) 
--       ON DELETE SET NULL
--   );
  

-- CREATE TABLE new_order_items
--   (
--     order_id   NUMBER( 12, 0 )                                , -- fk
--     item_id    NUMBER( 12, 0 )                                ,
--     product_id NUMBER( 12, 0 ) NOT NULL                       , -- fk
--     quantity   NUMBER( 8, 2 ) NOT NULL                        ,
--     unit_price NUMBER( 8, 2 ) NOT NULL                        ,
--     CONSTRAINT pk_new_order_items 
--       PRIMARY KEY( order_id, item_id ),
--     CONSTRAINT fk_new_order_items_products 
--       FOREIGN KEY( product_id )
--       REFERENCES products( product_id ) 
--       ON DELETE CASCADE,
--     CONSTRAINT fk_new_order_items_orders 
--       FOREIGN KEY( order_id )
--       REFERENCES new_orders( order_id ) 
--       ON DELETE CASCADE
--   );
    
    