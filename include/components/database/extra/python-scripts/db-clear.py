import psycopg2


# Connect to the PostgreSQL database
connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

# Create a cursor object to execute SQL queries
cursor = connection.cursor()

# Define SQL statements to create tables

clear_tables = """
TRUNCATE TABLE clients, passwords, tokens, forms, producers, views, films, producers_by_film
"""

cursor.execute(clear_tables)


# Commit the transaction
connection.commit()

# Close the cursor and connection
cursor.close()
connection.close()
