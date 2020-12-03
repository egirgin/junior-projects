USE `dbtify`;
DROP procedure IF EXISTS `songs_per_like`;

DELIMITER $$
USE `dbtify`$$
CREATE PROCEDURE `songs_per_like`()
BEGIN
drop table if exists temp_songs_per_like;
drop table if exists temp_song_artist;

create table dbtify.temp_songs_per_like
select count(listener_id) as likes, listener_song.song_id, title from listener_song 
inner join song on song.id = listener_song.song_id
group by listener_song.song_id;

create table dbtify.temp_song_artist
select song_artist.song_id, artist.id as artist_id, artist.name, artist.surname from song_artist
inner join artist on song_artist.artist_id = artist.id;

select temp_song_artist.song_id, title, likes, artist_id, name, surname from temp_song_artist
inner join temp_songs_per_like on temp_songs_per_like.song_id = temp_song_artist.song_id
order by likes desc;


drop table if exists temp_songs_per_like;
drop table if exists temp_song_artist;

END$$

DELIMITER ;

