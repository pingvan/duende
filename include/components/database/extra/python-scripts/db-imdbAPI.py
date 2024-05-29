import imdb
import psycopg2

ia = imdb.IMDb()

conn = psycopg2.connect("postgres://root:strongpassword@localhost:5432/duende")
cursor = conn.cursor()

def get_movie_cover(movie_name, filmed_year):
    search_results = ia.search_movie(movie_name)
    
    for movie in search_results:
        if movie.get('year') == filmed_year:
            ia.update(movie)
            cover_url = movie.get_fullsizeURL()
            return movie['cover url'], cover_url



get_films_query = "SELECT title, release_date FROM films WHERE cover IS NULL"
insert_image_urls_query = "UPDATE films SET cover_preview_url = %s, cover_full_size_url = %s WHERE title = %s"

cursor.execute(get_films_query)
films = cursor.fetchall()

for film in films:
    title = film[0]
    release_date = film[1]
    cover_url, cover_full_size_url = get_movie_cover(title, release_date.year)
    print(cover_url, cover_full_size_url, title)
    cursor.execute(insert_image_urls_query, (cover_url, cover_full_size_url, title))

conn.commit()
conn.close()

        