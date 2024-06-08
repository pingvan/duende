import psycopg2

connection = psycopg2.connect(
    "postgres://root:strongpassword@localhost:5432/duende"
)

cursor = connection.cursor()

create_genre_type = """DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'genre') THEN
        CREATE TYPE genre AS ENUM ('action', 'adventure', 'comedy', 'drama', 'horror', 'sci-fi', 'fantasy', 'thriller',
    'mystery', 'romance', 'animation', 'documentary',
    'crime', 'western', 'musical', 'history', 'war', 'music',
    'biographical', 'sports', 'family', 'foreign', 'tv movie');
    END IF;
END $$
"""

create_invite_status_type = """DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'invite_status') THEN
        CREATE TYPE invite_status AS ENUM ('sent', 'accepted', 'declined');
    END IF;
END $$
"""

create_event_types = """DO $$
BEGIN
    IF NOT EXISTS (SELECT 1 FROM pg_type WHERE typname = 'event_types') THEN
        CREATE TYPE event_type AS ENUM ('name_changed', 'description_changed', 'cover_changed', 'user_entered', 'user_left', 'user_removed');
    END IF;
END $$
"""

cursor.execute(create_genre_type)
cursor.execute(create_invite_status_type)
cursor.execute(create_event_types)

f = open('../../schema/sql-queries/sql_queries.sql', 'r')
sql = f.read()
cursor.execute(sql)

connection.commit()

cursor.close()
connection.close()
