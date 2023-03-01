create table if not exists planets
(
    id       serial primary key,
    name     varchar,
    distance integer
);

create table if not exists spacecrafts
(
    id       serial primary key,
    name     varchar,
    capacity integer
);

create table if not exists flights
(
    num           integer primary key,
    planet_id     integer not null references planets (id),
    spacecraft_id integer not null references spacecrafts (id)
);

create table if not exists tickets
(
    flight_num integer not null references flights (num),
    pax_name   varchar,
    price      integer
);

-- populate
insert into planets
values
    (default, 'Mercury', 58),
    (default, 'Venus', 108),
    (default, 'Earth', 150),
    (default, 'Mars', 228),
    (default, 'Jupyter', 778),
    (default, 'Saturn', 1430),
    (default, 'Uranus', 2870),
    (default, 'Neptune', 4500);
insert into spacecrafts
values
    (default, 'Alpha', 3),
    (default, 'Bravo', 1),
    (default, 'Charlie', 4),
    (default, 'Delta', 1),
    (default, 'Echo', 5),
    (default, 'Foxtrot', 9),
    (default, 'Golf', 2),
    (default, 'Hotel', 6);
insert into flights
values
    (1234, 1, 6),
    (2023, 3, 2),
    (8888, 2, 1),
    (7777, 3, 7),
    (1793, 6, 4),
    (1861, 7, 2),
    (5050, 8, 3),
    (8080, 4, 4);
insert into tickets
values
    (1234, 'Alice', 1),
    (1234, 'Bob', 2),
    (2023, 'Charles', 3),
    (8888, 'Denis', 4),
    (1861, 'Emil', 5),
    (1861, 'Fabio', 6),
    (5050, 'Greg', 7),
    (8080, 'Helen', 8),
    (8888, 'Ida', 9);
